//
// Created by Mohamed El Zarei on 13/12/2021.
//

#include "codegen/codegen.h"

#include <codegen/cpp.h>
#include <codegen/llvm.h>

namespace ZRegex {
  void Codegen::JIT() {
    llvm::SMDiagnostic Err;
    std::string fnName;

    if (backend_type_ == CPP) {
      module = llvm::parseIRFile(fmt::format("/tmp/{}.ll", filename_), Err, *context.getContext());
      for (auto &m : module->getFunctionList()) {
        auto fn = m.getName().str();
        if (fn.find("traverse") != std::string::npos) {
          fnName = fn;
          break;
        }
      }
    }

    auto err = jit->addModule(std::move(module));

    if (backend_type_ == CPP) {
      traverse_ptr
          = reinterpret_cast<bool (*)(char *, uint32_t n)>(jit->getPointerToFunction("_" + fnName));
    } else {
      traverse_ptr
          = reinterpret_cast<bool (*)(char *, uint32_t n)>(jit->getPointerToFunction("traverse"));
    }
  }
  bool Codegen::Run(const char *inp) { return traverse_ptr(const_cast<char *>(inp), strlen(inp)); }
  jit_func_t Codegen::GetFnPtr() { return traverse_ptr; }
  void Codegen::GenerateAndCompileCpp(std::unique_ptr<FiniteAutomaton> dfa, const char *filename) {
    // (1) Generate the cpp
    this->filename_ = filename;
    CppCodeGen::Generate(std::move(dfa), filename, encoding_);
    // (2) Call clang from system to compile the file to LLVM IR
    // this is very error-prone but works for our case here as prototype
    auto cmd = fmt::format("cd /tmp && clang -std=c++14 -O3 -emit-llvm {}.cpp -o {}.ll -S",
                           filename, filename);
    auto exit_code = system(cmd.c_str());
    if (exit_code) {
      // handle bad exit code
    }
  }
  void Codegen::GenerateAndCompileLLVM(std::unique_ptr<FiniteAutomaton> dfa) {
    ZRegex::LLVMCodeGen llvm(context, encoding_);
    llvm.Generate(std::move(dfa));
    module = llvm.TakeModule();
  }
  void Codegen::Compile(const char *pattern) {
    if (traverse_ptr != nullptr) return;
    auto dfa = RegExp::GetAutomatonForPattern(pattern);
    spdlog::info("Compiling pattern {}", pattern);
    if (backend_type_ == CPP) {
      GenerateAndCompileCpp(std::move(dfa), "regex");
    } else {
      GenerateAndCompileLLVM(std::move(dfa));
    }
    JIT();
  }

}  // namespace ZRegex