//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_LLVM_H
#define ZREGEXSTANDALONE_LLVM_H

#include <codegen/codegen.h>
#include <llvm/IR/IRBuilder.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <memory>
#include <string>

#include "fa/fa.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "opts.h"
#include "spdlog/fmt/fmt.h"

namespace ZRegex {
  class LLVMCodeGen {
  private:
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::map<uint32_t, llvm::BasicBlock*> states_to_blocks;
    std::unique_ptr<llvm::Module> module;
    llvm::orc::ThreadSafeContext context;
    CodegenOpts opts_;
    std::map<std::string, llvm::Function*> functionNamesToFns;

    llvm::BasicBlock* GetOrCreateBlock(uint32_t k, llvm::Function* parent);
    llvm::BasicBlock* GenerateTransition(uint32_t from, const FiniteAutomatonTransition& t,
                                         llvm::Value* c, llvm::Function* parent,
                                         llvm::BasicBlock* blk);
    void GenerateState(FiniteAutomatonState& s, llvm::Function* parent, llvm::Value* idx,
                       llvm::Value* str, llvm::Value* n, bool initial = false);

    void GenerateTraverse(std::unique_ptr<FiniteAutomaton> dfa);

    // Utf-8 Handling
    void GenerateMultiByteSequenceLength();
    void GenerateNextByte();
    void GenerateReadMultiByte();

  public:
    explicit LLVMCodeGen(llvm::orc::ThreadSafeContext& context, const CodegenOpts& opts)
        : context(context), opts_(opts) {
      builder = std::make_unique<llvm::IRBuilder<>>(*context.getContext());
      module = std::make_unique<llvm::Module>("rgx_module", *context.getContext());
    }

    // Main Generator
    void Generate(std::unique_ptr<FiniteAutomaton> dfa);

    // Helpers
    std::unique_ptr<llvm::Module> TakeModule() { return std::move(module); }
  };

}  // namespace ZRegex
#endif  // ZREGEXSTANDALONE_LLVM_H
