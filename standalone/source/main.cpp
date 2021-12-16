#include <parser/AutomataVisitor.h>
#include <codegen/codegen.h>
#include <codegen/llvmCodegen.h>
#include <parser/RegExp.h>
#include <zregex/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>

using namespace std;
auto main(int argc, char** argv) -> int {
  std::string s(argv[1]);
  Codegen::generate(s);

  LLVMCodeGen llvm;
  auto p = std::string("[a-a]");
  llvm.compile(p);
  llvm.run(s);
//  llvm.run("ab");
//  llvm.run("b");
  return 0;
}
