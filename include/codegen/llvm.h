//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_LLVM_H
#define ZREGEXSTANDALONE_LLVM_H

#include <llvm/IR/IRBuilder.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <string>

#include "../automaton/Automaton.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "spdlog/fmt/fmt.h"

namespace ZRegex {
  class LLVMCodeGen {
  private:
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::map<uint32_t, llvm::BasicBlock*> states_to_blocks;
    std::unique_ptr<llvm::Module> module;
    llvm::orc::ThreadSafeContext context;

    llvm::BasicBlock* GetOrCreateBlock(uint32_t k, llvm::Function* parent);
    llvm::BasicBlock* GenerateTransition(uint32_t from, Transition& t, llvm::Value* c,
                                         llvm::Function* parent, llvm::BasicBlock* blk);
    void GenerateState(State& s, llvm::Function* parent, llvm::Value* idx, llvm::Value* str,
                       llvm::Value* n, bool initial = false);

  public:
    explicit LLVMCodeGen(llvm::orc::ThreadSafeContext& context) : context(context) {
      builder = std::make_unique<llvm::IRBuilder<>>(*context.getContext());
      module = std::make_unique<llvm::Module>("rgx_module", *context.getContext());
    }
    void Generate(Automaton* dfa);
    std::unique_ptr<llvm::Module> TakeModule() { return std::move(module); }
  };

}  // namespace ZRegex
#endif  // ZREGEXSTANDALONE_LLVM_H
