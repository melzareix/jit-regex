//
// Created by Mohamed El Zarei on 15/12/2021.
//

#ifndef ZREGEX_LLVMCODEGEN_H
#define ZREGEX_LLVMCODEGEN_H

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/IRTransformLayer.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/ExecutionEngine/RTDyldMemoryManager.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Mangler.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

#include <map>
#include <memory>
#include <string>

#include "automaton/Automaton.h"
#include "codegen/jit.h"
#include "parser/AutomataVisitor.h"
#include "parser/RegExp.h"
#include "spdlog/spdlog.h"

class LLVMCodeGen {
private:
  llvm::orc::ThreadSafeContext context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<Automaton> automaton;
  std::map<uint32_t, llvm::BasicBlock*> states_to_blocks;
  std::unique_ptr<moderndbs::JIT> jit;
  bool (*fnPtr)(char* string, uint64_t i){};

  llvm::BasicBlock* get_or_create_block(uint32_t k, llvm::Function* parent) {
    if (auto it{states_to_blocks.find(k)}; it == std::end(states_to_blocks)) {
      states_to_blocks[k]
          = llvm::BasicBlock::Create(*context.getContext(), "s" + std::to_string(k), parent);
    }
    return states_to_blocks[k];
  }

  llvm::BasicBlock* transition_(uint32_t from, Transition& t, llvm::Value* c,
                                llvm::Function* parent) {
    auto l = t.min(), h = t.max();
    auto to = t.dest();
    spdlog::info("Transition [{}, {}] -> State{}", l, h, to);
    // single condition
    // if (c == %1) br %s1;
    if (l == h) {
      auto eq = builder->CreateICmpEQ(c, builder->getInt8(l));
      auto next_state = llvm::BasicBlock::Create(*context.getContext(),
                                                 "s" + std::to_string(from) + "_cnt", parent);
      auto true_state = get_or_create_block(to, parent);
      builder->CreateCondBr(eq, true_state, next_state);
      return next_state;
    }
    return nullptr;
  }

  void state_(State& s, llvm::Function* parent, llvm::Value* idx, llvm::Value* str, llvm::Value* n,
              bool initial = false) {
    auto ctx = context.getContext();
    llvm::BasicBlock* stateBlockEntry = get_or_create_block(s.id(), parent);

    if (initial) {
      builder->CreateBr(stateBlockEntry);
    }
    builder->SetInsertPoint(stateBlockEntry);

    // easy case accept just return
    if (s.is_accept()) {
      builder->SetInsertPoint(stateBlockEntry);
      builder->CreateRet(builder->getInt1(true));
      return;
    }

    // bound check
    // compare local variable idx > n(str sz)
    // https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl07.html#id4

    auto bchk_t
        = llvm::BasicBlock::Create(*ctx, "bounds_then", builder->GetInsertBlock()->getParent());
    auto bchk_cnt
        = llvm::BasicBlock::Create(*ctx, "bounds_cont", builder->GetInsertBlock()->getParent());

    auto load_idx = builder->CreateLoad(builder->getInt64Ty(), idx);
    auto cmp = builder->CreateICmp(llvm::CmpInst::ICMP_UGE, load_idx, n);
    builder->CreateCondBr(cmp, bchk_t, bchk_cnt);

    // idx > n -> ret false
    builder->SetInsertPoint(bchk_t);
    builder->CreateRet(builder->getInt1(false));

    // continue after bounds check
    builder->SetInsertPoint(bchk_cnt);

    // c = str[idx]
    auto c_mem = builder->CreateGEP(str, load_idx);
    auto c = builder->CreateLoad(builder->getInt8Ty(), c_mem);
    // idx++
    auto inc = builder->CreateAdd(load_idx, builder->getInt64(1));
    builder->CreateStore(inc, idx);

    spdlog::info("State{}: Transitions Size: {}", s.id(), s.transitions().size());

    llvm::BasicBlock* last_block;
    for (auto t : s.transitions()) {
      last_block = transition_(s.id(), t, c, parent);
    }
    if (last_block != nullptr) {
      builder->SetInsertPoint(last_block);
      builder->CreateRet(builder->getInt1(false));
    }
  }

  void traverse_() {
    auto ctx = context.getContext();

    // i1 @traverse_(i8* %str, i32 %str_length)
    auto bool_ret_ty = llvm::Type::getInt1Ty(*ctx);

    auto str_ptr_ty = llvm::Type::getInt8PtrTy(*ctx);
    auto str_sz_ty = llvm::Type::getInt64Ty(*ctx);

    auto* traverse_fn_ty = llvm::FunctionType::get(bool_ret_ty, {str_ptr_ty, str_sz_ty}, false);
    auto traverse_fn = llvm::cast<llvm::Function>(
        module->getOrInsertFunction("traverse", traverse_fn_ty).getCallee());

    // entry:
    llvm::BasicBlock* traverseFuncBlockEntry = llvm::BasicBlock::Create(*ctx, "entry", traverse_fn);
    builder->SetInsertPoint(traverseFuncBlockEntry);

    // int idx = 0
    auto idx_var = builder->CreateAlloca(llvm::Type::getInt64Ty(*ctx), nullptr, "idx");
    builder->CreateStore(builder->getInt64(0), idx_var);

    auto initial_state = automaton->states[automaton->initial()];

    state_(initial_state, traverse_fn, idx_var, &*traverse_fn->arg_begin(),
           &*(traverse_fn->arg_begin() + 1), true);
    //
    for (auto& s : automaton->states) {
      if (s.first == initial_state.id()) continue;
      state_(s.second, traverse_fn, idx_var, &*traverse_fn->arg_begin(),
             &*(traverse_fn->arg_begin() + 1), false);
    }
  }

  void test() {
    auto ctx = context.getContext();
    auto ft = llvm::Type::getInt64Ty(*ctx);
    auto* funcT = llvm::FunctionType::get(ft, false);
    auto funcFn
        = llvm::cast<llvm::Function>(module->getOrInsertFunction("func", funcT).getCallee());
    llvm::BasicBlock* funcBlockEntry = llvm::BasicBlock::Create(*ctx, "entry", funcFn);
    builder->SetInsertPoint(funcBlockEntry);
    builder->CreateRet(builder->getInt64(1));
  }
  void compile_pattern(const std::string& pattern) {
    RegExp regexp(pattern);
    auto root = regexp.parse();
    automaton = std::make_unique<Automaton>(*AutomataVisitor::GetValue(root));
    if (automaton == nullptr) {
      throw std::runtime_error("Failure! Regexp compilation failed!.");
    }

    // TODO(melzarei) remove expand for all automatons
    automaton->expand();

    if (!automaton->is_dfa()) {
      throw std::runtime_error("Failure! Cannot generate code from NFA! Automaton must be DFA!.");
    }
  }

public:
  LLVMCodeGen() {
    // JIT initializers
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

    // Initializers
    context = {std::make_unique<llvm::LLVMContext>()};
    module = std::make_unique<llvm::Module>("rgx_module", *context.getContext());
    builder = std::make_unique<llvm::IRBuilder<>>(*context.getContext());
    fnPtr = nullptr;
    jit = std::make_unique<moderndbs::JIT>(context);
  }

  void compile(std::string& pattern) {
    this->compile_pattern(pattern);
    this->traverse_();

    auto e = jit->addModule(std::move(module));
    fnPtr = reinterpret_cast<bool (*)(char*, uint64_t n)>(jit->getPointerToFunction("traverse"));
  }

  void run(const std::string& sss) {
    auto sz = sss.size();
    spdlog::info("Result: {}", fnPtr(const_cast<char*>(sss.c_str()), sz));
  }
};

#endif  // ZREGEX_LLVMCODEGEN_H
