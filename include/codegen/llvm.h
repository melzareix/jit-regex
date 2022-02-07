//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_LLVM_H
#define ZREGEXSTANDALONE_LLVM_H

#include <llvm/IR/IRBuilder.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <memory>
#include <string>

#include "fa/fa.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "spdlog/fmt/fmt.h"

namespace ZRegex {
  class LLVMCodeGen {
  private:
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::map<uint32_t, llvm::BasicBlock*> states_to_blocks;
    std::unique_ptr<llvm::Module> module;
    llvm::orc::ThreadSafeContext context;
    std::map<std::string, llvm::Function*> functionNamesToFns;

    llvm::BasicBlock* GetOrCreateBlock(uint32_t k, llvm::Function* parent);
    llvm::BasicBlock* GenerateTransition(uint32_t from, const FiniteAutomatonTransition& t, llvm::Value* c,
                                         llvm::Function* parent, llvm::BasicBlock* blk);
    void GenerateState(FiniteAutomatonState& s, llvm::Function* parent, llvm::Value* idx,
                       llvm::Value* str, llvm::Value* n, bool initial = false);
  public:
    explicit LLVMCodeGen(llvm::orc::ThreadSafeContext& context) : context(context) {
      builder = std::make_unique<llvm::IRBuilder<>>(*context.getContext());
      module = std::make_unique<llvm::Module>("rgx_module", *context.getContext());
    }
    void GenerateMultiByteSequenceLength();
    void GenerateNextByte();
    void GenerateReadMultiByte();
    void Generate(std::unique_ptr<FiniteAutomaton> dfa);
    std::unique_ptr<llvm::Module> TakeModule() { return std::move(module); }
  };

}  // namespace ZRegex
#endif  // ZREGEXSTANDALONE_LLVM_H
