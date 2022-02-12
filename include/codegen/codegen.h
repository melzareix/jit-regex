//
// Created by Mohamed ElZarei on 13/12/2021.
//

#pragma once
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>

#include <fstream>
#include <memory>
#include <string>

#include "../parser/RegExp.h"
#include "fa/fa.h"
#include "jit.h"
#include "llvm/ExecutionEngine/Orc/Mangling.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "spdlog/spdlog.h"

namespace ZRegex {
  enum CodegenBackendType { LLVM, CPP };
  enum Encoding { UTF8, ASCII };
  typedef bool (*jit_func_t)(char*, uint64_t);
  class Codegen {
  private:
    CodegenBackendType backend_type_;
    Encoding encoding_;
    const char* filename_;
    jit_func_t traverse_ptr;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<ZRegex::JIT> jit;
    llvm::orc::ThreadSafeContext context;

    void GenerateAndCompileCpp(const std::unique_ptr<FiniteAutomaton> dfa, const char* filename);
    void GenerateAndCompileLLVM(const std::unique_ptr<FiniteAutomaton> dfa);
    void JIT();
    jit_func_t GetFnPtr();

  public:
    Codegen(const CodegenBackendType& backend_type, const Encoding& encoding)
        : backend_type_(backend_type),
          traverse_ptr(nullptr),
          filename_(nullptr),
          encoding_(encoding),
          context({std::make_unique<llvm::LLVMContext>()}) {
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
      jit = std::make_unique<ZRegex::JIT>(context);
    }

    void Compile(const char* pattern);
    bool Run(const char* pattern);
  };
}  // namespace ZRegex