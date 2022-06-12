//
// Created by Mohamed El Zarei on 13/12/2021.
//

#include "codegen/codegen.h"

#include <codegen/cpp.h>
#include <codegen/llvm.h>
#include <fa/special/kmp.h>

#include "parser/RegExp.h"

namespace ZRegex {
  void Codegen::JIT_cpp() {
    llvm::SMDiagnostic Err;
    std::string fnName;

    module = llvm::parseIRFile(fmt::format("/tmp/{}.ll", filename_), Err, *context.getContext());
    for (auto &m : module->getFunctionList()) {
      auto fn = m.getName().str();
      // spdlog::debug("Fn {}", fn);
      if (fn.find("traverse") != std::string::npos) {
        fnName = fn;
        break;
      }
    }
    auto err = jit->addModule(std::move(module));
    traverse_ptr
        = reinterpret_cast<bool (*)(char *, uint32_t n)>(jit->getPointerToFunction(fnName));
  }

  void Codegen::JIT() {
    if (opts_.GetBackendType() == CodegenOpts::CodegenBackendType::CPP) {
      JIT_cpp();
      return;
    }

    auto err = jit->addModule(std::move(module));
    traverse_ptr
        = reinterpret_cast<bool (*)(char *, uint32_t n)>(jit->getPointerToFunction("traverse"));
  }

  bool Codegen::Run(const char *inp) { return traverse_ptr(const_cast<char *>(inp), strlen(inp)); }
  jit_func_t Codegen::GetFnPtr() { return traverse_ptr; }
  void Codegen::GenerateAndCompileCpp(std::unique_ptr<FiniteAutomaton> dfa, std::string pattern,
                                      const char *filename) {
    // (1) Generate the cpp
    this->filename_ = filename;
    CppCodeGen cppGenerator(std::move(dfa), opts_, filename);
    cppGenerator.Generate();

    // (2) Call clang from system to compile the file to LLVM IR
    // this is very error-prone but works for our case here as prototype
    auto cmd = fmt::format("cd /tmp && clang-13 -std=c++14 -{} -emit-llvm {}.cpp -o {}.ll -S",
                           opts_.getOptimizationLevelString(), filename, filename);
    auto exit_code = system(cmd.c_str());
    if (exit_code) {
      // handle bad exit code
    }
  }
  void Codegen::GenerateAndCompileLLVM(std::unique_ptr<FiniteAutomaton> dfa) {
    ZRegex::LLVMCodeGen llvm(context, opts_);
    llvm.Generate(std::move(dfa));
    module = llvm.TakeModule();
  }
  void Codegen::CompileKMP(const char *pattern) {
    if (traverse_ptr != nullptr) return;
    auto kmp = ZRegex::KMPAlgorithm(pattern);
    auto dfa = kmp.asDfa();
    if (opts_.GetBackendType() == CodegenOpts::CodegenBackendType::CPP) {
      GenerateAndCompileCpp(std::move(dfa), std::string(pattern), "regex");
    } else {
      GenerateAndCompileLLVM(std::move(dfa));
    }
    JIT();
  }

  void Codegen::GenerateCodeAndCompile(std::unique_ptr<FiniteAutomaton> dfa,
                                       const std::string &pattern) {
    if (opts_.GetBackendType() == CodegenOpts::CodegenBackendType::CPP) {
      GenerateAndCompileCpp(std::move(dfa), std::string(pattern), "regex");
    } else {
      GenerateAndCompileLLVM(std::move(dfa));
    }
    FiniteAutomatonState::ResetCounter();
    JIT();
  }
  void Codegen::Compile(const char *pattern) {
    auto dfa = RegExp::GetAutomatonForPattern(pattern, opts_.IsByteDFA());
    if (opts_.GetBackendType() == CodegenOpts::CodegenBackendType::CPP) {
      GenerateAndCompileCpp(std::move(dfa), std::string(pattern), "regex");
    } else {
      GenerateAndCompileLLVM(std::move(dfa));
    }
    FiniteAutomatonState::ResetCounter();
    JIT();
  }

  void Codegen::CompileForBenchmark(const char *pattern) {
    traverse_ptr = nullptr;
    Compile(pattern);
  }
}  // namespace ZRegex