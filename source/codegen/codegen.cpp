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
      module = llvm::parseIRFile(fmt::format("/tmp/{}.ll", filename), Err, *context.getContext());
      for (auto& m : module->getFunctionList()) {
        auto fn = m.getName().str();
        if (fn.find("traverse") != std::string::npos) {
          fnName = fn;
          break;
        }
      }
    }
    auto err = jit->addModule(std::move(module));

    if (backend_type_ == CPP) {
      traverse_ptr = reinterpret_cast<bool (*)(char *, uint64_t n)>(
          jit->
          getPointerToFunction("_" + fnName));
    } else {
      // mbs
      const char* str = u8"💙";
//      const char* str = u8"a";
      auto z = reinterpret_cast<uint8_t (*)(uint8_t)>(jit->getPointerToFunction("multiByteSequenceLength"));
      std::cout << "hello" << std::endl;
      auto x = z(str[0]);
      std::cout << std::to_string(x) << std::endl;

      //readMultiByte
      auto y = reinterpret_cast<unsigned (*)(char*, char, unsigned, unsigned)>(jit->getPointerToFunction("readMultiByte"));
      std::cout << "hello2" << std::endl;
//      const char* str = u8"ꤰ";
      auto ff = y(const_cast<char *>(str), str[0], 0, x);
      std::cout << std::to_string(ff) << std::endl;

      // nextByte
      auto nextByte = reinterpret_cast<unsigned (*)(char*, unsigned*)>(jit->getPointerToFunction("nextByte"));
      std::cout << "hello3" << std::endl;
      unsigned idxxx = 0;
      auto fz = nextByte(const_cast<char *>(str), &idxxx);
      std::cout << std::to_string(fz) << std::endl;
      std::cout << std::to_string(idxxx) << std::endl;

      traverse_ptr
          = reinterpret_cast<bool (*)(char *, uint64_t n)>(jit->getPointerToFunction("traverse"));
    }
  }
  bool Codegen::Run(const char *inp) {
    return traverse_ptr(const_cast<char *>(inp), strlen(inp));
  }
  jit_func_t Codegen::GetFnPtr() { return traverse_ptr; }
  void Codegen::GenerateAndCompileCpp(std::unique_ptr<FiniteAutomaton> dfa, const char *filename) {
    // (1) Generate the cpp
    this->filename = filename;
    CppCodeGen::Generate(std::move(dfa), filename);
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
    ZRegex::LLVMCodeGen llvm(context);
    llvm.GenerateMultiByteSequenceLength();
    llvm.GenerateReadMultiByte();
    llvm.GenerateNextByte();
    llvm.Generate(std::move(dfa));
    module = llvm.TakeModule();
  }
  void Codegen::Compile(const char *pattern) {
    auto dfa = RegExp::GetAutomatonForPattern(pattern);

    if (backend_type_ == CPP) {
      GenerateAndCompileCpp(std::move(dfa), "regex");
    } else {
      GenerateAndCompileLLVM(std::move(dfa));
    }
    JIT();
  }

}  // namespace ZRegex