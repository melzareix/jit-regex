//
// Created by Mohamed El Zarei on 02/01/2022.
//

#include "codegen/llvm.h"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Utils/Cloning.h>

using namespace llvm;
namespace ZRegex {

  void LLVMCodeGen::GenerateMultiByteSequenceLength() {
    // multiByteSequenceLength fn
    auto ctx = context.getContext();
    auto ret_ty = llvm::Type::getInt8Ty(*ctx);
    auto first_byte_ty = llvm::Type::getInt8Ty(*ctx);
    auto* clz_fn_ty = llvm::FunctionType::get(ret_ty, {first_byte_ty}, false);
    auto multibyte_seq_len_fn = llvm::cast<llvm::Function>(
        module->getOrInsertFunction("multiByteSequenceLength", clz_fn_ty).getCallee());
    functionNamesToFns["multiByteSequenceLength"] = multibyte_seq_len_fn;
    llvm::BasicBlock* fnEntry = llvm::BasicBlock::Create(*ctx, "entry", multibyte_seq_len_fn);
    builder->SetInsertPoint(fnEntry);
    auto neg = builder->CreateXor(&*multibyte_seq_len_fn->arg_begin(), builder->getInt8(-1));
    auto len = builder->CreateBinaryIntrinsic(llvm::Intrinsic::ctlz, neg, builder->getInt1(false));

    auto cond = builder->CreateICmpUGT(len, builder->getInt8(0));
    auto byteLen = builder->CreateSelect(cond, len, builder->getInt8(1));
    builder->CreateRet(byteLen);
  }
  void LLVMCodeGen::GenerateNextByte() {
    auto ctx = context.getContext();
    auto ret_ty = llvm::Type::getInt32Ty(*ctx);
    auto str_ptr_ty = llvm::Type::getInt8PtrTy(*ctx);
    auto idx_ty = llvm::Type::getInt32PtrTy(*ctx);
    auto* clz_fn_ty = llvm::FunctionType::get(ret_ty, {str_ptr_ty, idx_ty}, false);
    auto next_byte_fn = llvm::cast<llvm::Function>(
        module->getOrInsertFunction("nextByte", clz_fn_ty).getCallee());
    functionNamesToFns["nextByte"] = next_byte_fn;
    llvm::BasicBlock* fnEntry = llvm::BasicBlock::Create(*ctx, "entry", next_byte_fn);
    builder->SetInsertPoint(fnEntry);

    auto str = &*(next_byte_fn->arg_begin());
    auto idx = &*(next_byte_fn->arg_begin() + 1);
    auto load_idx = builder->CreateLoad(builder->getInt32Ty(), idx);

    // str[idx]
    auto c_mem = builder->CreateGEP(str, load_idx);
    auto firstByte = builder->CreateLoad(builder->getInt8Ty(), c_mem);
    // byteLen
    auto byteLen = builder->CreateCall(functionNamesToFns["multiByteSequenceLength"], {firstByte});

    auto tzext = builder->CreateZExt(byteLen, builder->getInt32Ty());
    //    builder->CreateRet(tzext);
    // c = readMultiByte(str, fb, idx, byteLen);
    auto c = builder->CreateCall(functionNamesToFns["readMultiByte"],
                                 {str, firstByte, load_idx, byteLen});
    auto idxInc = builder->CreateAdd(load_idx, tzext);
    builder->CreateStore(idxInc, idx);
    builder->CreateRet(c);
  }

  void LLVMCodeGen::GenerateReadMultiByte() {
    auto ctx = context.getContext();
    auto ret_ty = llvm::Type::getInt32Ty(*ctx);
    auto first_byte_ty = llvm::Type::getInt8Ty(*ctx);
    auto str_ptr_ty = llvm::Type::getInt8PtrTy(*ctx);
    auto idx_ty = llvm::Type::getInt32Ty(*ctx);
    auto bytelen_ty = llvm::Type::getInt8Ty(*ctx);
    auto* clz_fn_ty
        = llvm::FunctionType::get(ret_ty, {str_ptr_ty, first_byte_ty, idx_ty, bytelen_ty}, false);
    auto read_mb_fn = llvm::cast<llvm::Function>(
        module->getOrInsertFunction("readMultiByte", clz_fn_ty).getCallee());
    functionNamesToFns["readMultiByte"] = read_mb_fn;
    llvm::BasicBlock* fnEntry = llvm::BasicBlock::Create(*ctx, "entry", read_mb_fn);
    builder->SetInsertPoint(fnEntry);
    // switch
    auto str = &*(read_mb_fn->arg_begin());
    auto fb = &*(read_mb_fn->arg_begin() + 1);
    auto idx = &*(read_mb_fn->arg_begin() + 2);
    auto cond = &*(read_mb_fn->arg_begin() + 3);
    auto defaultBlock = llvm::BasicBlock::Create(*context.getContext(), "default", read_mb_fn);

    auto switchOp = builder->CreateSwitch(cond, defaultBlock, 3);

    // default
    builder->SetInsertPoint(defaultBlock);
    auto fb_zext = builder->CreateZExt(fb, builder->getInt32Ty());
    builder->CreateRet(fb_zext);

    // byteLen = 2
    auto lenTwoBlock = llvm::BasicBlock::Create(*context.getContext(), "byteLenTwo", read_mb_fn);
    switchOp->addCase(builder->getInt8(2), lenTwoBlock);
    builder->SetInsertPoint(lenTwoBlock);
    // firstByte & 0x1F
    auto p12 = builder->CreateAnd(fb, builder->getInt8(0x1F));
    auto p12_zext = builder->CreateZExt(p12, builder->getInt32Ty());
    auto p121 = builder->CreateShl(p12_zext, builder->getInt32(6));

    // str[idx + 1]
    auto idx_1 = builder->CreateAdd(idx, builder->getInt32(1));
    auto c_mem = builder->CreateGEP(str, idx_1);
    auto secondByte = builder->CreateLoad(builder->getInt8Ty(), c_mem);
    auto p2 = builder->CreateAnd(secondByte, builder->getInt8(0x3F));
    auto p2_zext = builder->CreateZExt(p2, builder->getInt32Ty());
    auto p1orp2 = builder->CreateOr(p121, p2_zext);
    builder->CreateRet(p1orp2);

    // byteLen = 3
    auto lenThreeBlock
        = llvm::BasicBlock::Create(*context.getContext(), "byteLenThree", read_mb_fn);
    switchOp->addCase(builder->getInt8(3), lenThreeBlock);
    builder->SetInsertPoint(lenThreeBlock);
    auto p13 = builder->CreateAnd(fb, builder->getInt8(0xF));
    auto p13_zext = builder->CreateZExt(p13, builder->getInt32Ty());
    auto p131 = builder->CreateShl(p13_zext, builder->getInt32(12));

    // str[idx + 1]
    auto idx_13 = builder->CreateAdd(idx, builder->getInt32(1));
    auto c_mem3 = builder->CreateGEP(str, idx_13);
    auto secondByte3 = builder->CreateLoad(builder->getInt8Ty(), c_mem3);

    auto p23 = builder->CreateAnd(secondByte3, builder->getInt8(0x3F));
    auto p23_zext = builder->CreateZExt(p23, builder->getInt32Ty());
    auto p231 = builder->CreateShl(p23_zext, builder->getInt32(6));

    // str[idx + 2]
    auto idx_23 = builder->CreateAdd(idx, builder->getInt32(2));
    auto c_mem23 = builder->CreateGEP(str, idx_23);
    auto thirdByte = builder->CreateLoad(builder->getInt8Ty(), c_mem23);

    auto p223 = builder->CreateAnd(thirdByte, builder->getInt8(0x3F));
    auto p223_zext = builder->CreateZExt(p223, builder->getInt32Ty());

    // or
    Value* ref[3] = {p131, p231, p223_zext};
    auto p1orp2orp3 = builder->CreateOr(ref);
    builder->CreateRet(p1orp2orp3);

    // byteLen = 4
    auto lenFourBlock = llvm::BasicBlock::Create(*context.getContext(), "byteLenFour", read_mb_fn);
    switchOp->addCase(builder->getInt8(4), lenFourBlock);
    builder->SetInsertPoint(lenFourBlock);

    // fb & 0x07
    auto p134 = builder->CreateAnd(fb, builder->getInt8(0x7));
    auto p134_zext = builder->CreateZExt(p134, builder->getInt32Ty());
    auto p141 = builder->CreateShl(p134_zext, builder->getInt32(18));

    // str[idx + 1]
    auto idx_14 = builder->CreateAdd(idx, builder->getInt32(1));
    auto c_mem4 = builder->CreateGEP(str, idx_14);
    auto secondByte4 = builder->CreateLoad(builder->getInt8Ty(), c_mem4);

    auto p24 = builder->CreateAnd(secondByte4, builder->getInt8(0x3F));
    auto p24_zext = builder->CreateZExt(p24, builder->getInt32Ty());
    auto p241 = builder->CreateShl(p24_zext, builder->getInt32(12));

    // str[idx + 2]
    auto idx_24 = builder->CreateAdd(idx, builder->getInt32(2));
    auto c_mem24 = builder->CreateGEP(str, idx_24);
    auto thirdByte4 = builder->CreateLoad(builder->getInt8Ty(), c_mem24);

    auto p224 = builder->CreateAnd(thirdByte4, builder->getInt8(0x3F));
    auto p224_zext = builder->CreateZExt(p224, builder->getInt32Ty());
    auto p243 = builder->CreateShl(p224_zext, builder->getInt32(6));

    // str[idx + 3]
    auto idx_34 = builder->CreateAdd(idx, builder->getInt32(3));
    auto c_mem34 = builder->CreateGEP(str, idx_34);
    auto fourthByte = builder->CreateLoad(builder->getInt8Ty(), c_mem34);

    auto p4 = builder->CreateAnd(fourthByte, builder->getInt8(0x3F));
    auto p4_zext = builder->CreateZExt(p4, builder->getInt32Ty());
    Value* ref4[4] = {p141, p241, p243, p4_zext};
    auto p1orp2orp3orp4 = builder->CreateOr(ref4);
    builder->CreateRet(p1orp2orp3orp4);
  }
  void LLVMCodeGen::Generate(std::unique_ptr<FiniteAutomaton> dfa) {
    if (encoding_ == UTF8) {
      this->GenerateMultiByteSequenceLength();
      this->GenerateReadMultiByte();
      this->GenerateNextByte();
    }
    this->GenerateTraverse(std::move(dfa));
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

    auto load_idx = builder->CreateLoad(builder->getInt32Ty(), idx);
    auto cmp = builder->CreateICmp(llvm::CmpInst::ICMP_UGE, load_idx, n);
    builder->CreateCondBr(cmp, bchk_t, bchk_cnt);

    // idx > n -> ret false
    builder->SetInsertPoint(bchk_t);
    builder->CreateRet(builder->getInt1(false));

    // continue after bounds check
    builder->SetInsertPoint(bchk_cnt);
    llvm::Value* c;
    if (encoding_ == UTF8) {
      c = builder->CreateCall(functionNamesToFns["nextByte"], {str, idx});
    } else {
      // c = str[idx]
      auto c_mem = builder->CreateGEP(str, load_idx);
      c = builder->CreateLoad(builder->getInt8Ty(), c_mem);
      // idx++
      auto inc = builder->CreateAdd(load_idx, builder->getInt32(1));
      builder->CreateStore(inc, idx);
    }

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
  llvm::BasicBlock* LLVMCodeGen::GenerateTransition(uint32_t from,
                                                    const FiniteAutomatonTransition& t,
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
    ConstantInt* low = builder->getInt32(l);
    ConstantInt* high = builder->getInt32(h);
    if (encoding_ == ASCII) {
      low = builder->getInt8(l);
      high = builder->getInt8(h);
    }
    // single condition
    // if (c == %1) br %s1;
    if (l == h) {
      auto eq = builder->CreateICmpEQ(c, low);
      builder->CreateCondBr(eq, true_state, next_state);
    } else {
      // if (c >= l && c <= h)
      auto cgel = builder->CreateICmpUGE(c, low);
      auto cleh = builder->CreateICmpULE(c, high);
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
  void LLVMCodeGen::GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa) {
    auto ctx = context.getContext();

    // i1 @traverse_(i8* %str, i32 %str_length)
    auto bool_ret_ty = llvm::Type::getInt1Ty(*ctx);

    auto str_ptr_ty = llvm::Type::getInt8PtrTy(*ctx);
    auto str_sz_ty = llvm::Type::getInt32Ty(*ctx);

    auto* traverse_fn_ty = llvm::FunctionType::get(bool_ret_ty, {str_ptr_ty, str_sz_ty}, false);
    auto traverse_fn = llvm::cast<llvm::Function>(
        module->getOrInsertFunction("traverse", traverse_fn_ty).getCallee());

    // entry:
    llvm::BasicBlock* traverseFuncBlockEntry = llvm::BasicBlock::Create(*ctx, "entry", traverse_fn);
    builder->SetInsertPoint(traverseFuncBlockEntry);

    // int idx = 0
    auto idx_var = builder->CreateAlloca(llvm::Type::getInt32Ty(*ctx), nullptr, "idx");
    builder->CreateStore(builder->getInt32(0), idx_var);

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
}  // namespace ZRegex