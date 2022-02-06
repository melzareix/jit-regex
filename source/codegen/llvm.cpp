//
// Created by Mohamed El Zarei on 02/01/2022.
//

#include "codegen/llvm.h"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>

namespace ZRegex {
  void LLVMCodeGen::Generate(std::unique_ptr<FiniteAutomaton> dfa) {
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

    auto initial_state = dfa->initial_state;
    auto states = dfa->GetStates();

    GenerateState(*initial_state, traverse_fn, idx_var, &*traverse_fn->arg_begin(),
                  &*(traverse_fn->arg_begin() + 1), true);
    //
    for (auto& s : states) {
      if (s->id == initial_state->id) continue;
      GenerateState(*s, traverse_fn, idx_var, &*traverse_fn->arg_begin(),
                    &*(traverse_fn->arg_begin() + 1), false);
    }
  }

  void LLVMCodeGen::GenerateState(FiniteAutomatonState& s, llvm::Function* parent, llvm::Value* idx,
                                  llvm::Value* str, llvm::Value* n, bool initial) {
    auto ctx = context.getContext();
    llvm::BasicBlock* stateBlockEntry = GetOrCreateBlock(s.id, parent);

    if (initial) {
      builder->CreateBr(stateBlockEntry);
    }
    builder->SetInsertPoint(stateBlockEntry);

    // easy case accept just return
    if (s.accept) {
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

    spdlog::debug("State{}: Transitions Size: {}", s.id, s.transitions.size());
    llvm::BasicBlock* last_block = nullptr;
    for (const auto& t : s.transitions) {
      last_block = GenerateTransition(s.id, t, c, parent, last_block);
    }
    if (last_block != nullptr) {
      builder->SetInsertPoint(last_block);
      builder->CreateRet(builder->getInt1(false));
    }
  }
  llvm::BasicBlock* LLVMCodeGen::GenerateTransition(uint32_t from, const FiniteAutomatonTransition& t,
                                                    llvm::Value* c, llvm::Function* parent,
                                                    llvm::BasicBlock* blk) {
    auto l = t.min, h = t.max;
    auto to = t.to;
    spdlog::debug("Transition [{}, {}] -> State{}", l, h, to->id);
    auto next_state = llvm::BasicBlock::Create(*context.getContext(),
                                               "s" + std::to_string(from) + "_nt", parent);
    auto true_state = GetOrCreateBlock(to->id, parent);

    if (blk != nullptr) {
      builder->SetInsertPoint(blk);
    }
    // single condition
    // if (c == %1) br %s1;
    if (l == h) {
      auto eq = builder->CreateICmpEQ(c, builder->getInt32(l));
      builder->CreateCondBr(eq, true_state, next_state);
    } else {
      // if (c >= l && c <= h)
      auto cgel = builder->CreateICmpUGE(c, builder->getInt32(l));
      auto cleh = builder->CreateICmpULE(c, builder->getInt32(h));
      auto cnd = builder->CreateAnd(cgel, cleh);
      builder->CreateCondBr(cnd, true_state, next_state);
    }

    return next_state;
  }
  llvm::BasicBlock* LLVMCodeGen::GetOrCreateBlock(uint32_t k, llvm::Function* parent) {
    if (auto it{states_to_blocks.find(k)}; it == std::end(states_to_blocks)) {
      states_to_blocks[k]
          = llvm::BasicBlock::Create(*context.getContext(), "s" + std::to_string(k), parent);
    }
    return states_to_blocks[k];
  }
}  // namespace ZRegex