//
// Created by Mohamed ElZarei on 13/12/2021.
//

#pragma once
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>

#include <fstream>
#include <memory>
#include <string>

#include "../automaton/Automaton.h"
#include "../parser/AutomataVisitor.h"
#include "../parser/RegExp.h"
#include "jit.h"
#include "llvm/ExecutionEngine/Orc/Mangling.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "spdlog/spdlog.h"

namespace ZRegex {
  enum CodegenBackendType { LLVM, CPP };
  typedef bool (*jit_func_t)(char*, uint64_t);
  class Codegen {
  private:
    CodegenBackendType backend_type_;
    jit_func_t traverse_ptr;
    const char* filename;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<ZRegex::JIT> jit;
    llvm::orc::ThreadSafeContext context;

    void GenerateAndCompileCpp(Automaton* dfa, const char* filename);
    void GenerateAndCompileLLVM(Automaton* dfa);
    void JIT();
    jit_func_t GetFnPtr();

  public:
    explicit Codegen(const CodegenBackendType&& backend_type)
        : backend_type_(backend_type),
          traverse_ptr(nullptr),
          context({std::make_unique<llvm::LLVMContext>()}) {
      spdlog::info("BackendType: {}", backend_type);
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

      jit = std::make_unique<ZRegex::JIT>(context);
    }

    void Compile(const char* pattern);
    bool Run(const char* pattern);

    //    void McJitTest(char* s, uint64_t sz) {
    //      llvm::InitializeNativeTarget();
    //      llvm::InitializeNativeTargetAsmPrinter();
    //      llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
    //
    //      llvm::orc::ThreadSafeContext context = {std::make_unique<llvm::LLVMContext>()};
    //      llvm::SMDiagnostic Err;
    //
    //      auto Mod = llvm::parseIRFile(
    //          "/Users/melzarei/Documents/code-compiling-regex/build/codegen_test.ll", Err,
    //          *context.getContext());
    //
    //      auto jit = std::make_unique<ZRegex::JIT>(context);
    //      auto err = jit->addModule(std::move(Mod));
    //      std::string mangledName;
    //      llvm::raw_string_ostream mangledNameStream(mangledName);
    //      llvm::Mangler::getNameWithPrefix(mangledNameStream, "traverse", jit->getDataLayout());
    //
    //      //      llvm::MangleAndInterner Mangle;
    //      auto const dl = jit->getDataLayout();
    //      llvm::orc::MangleAndInterner Mangle(jit->getES(), dl);
    //      auto name = Mangle("__Z8traversemPc1j");
    //      traverse_ptr = reinterpret_cast<bool (*)(uint64_t idx, char*, uint64_t n)>(
    //          jit->getPointerToFunction("__Z8trave1rsemPcj"));
    //      if (traverse_ptr == nullptr) {
    //        std::cout << "WHY" << std::endl;
    //      }
    //      auto r = traverse_ptr(0, s, sz);
    //      std::cout << r << std::endl;
    //    }
  };
}  // namespace ZRegex