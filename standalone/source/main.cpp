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
  std::string pattern(argv[1]);
  std::string inp(argv[2]);
  Codegen::generate(pattern);

  LLVMCodeGen llvm;
  auto p = std::string(pattern);
  llvm.compile(p);
  llvm.run(inp);
//  llvm.run("ab");
//  llvm.run("b");
  return 0;
}
