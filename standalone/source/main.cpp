#include <codegen/llvmCodegen.h>

#include <cxxopts.hpp>
#include <string>

using namespace std;
auto main(int argc, char** argv) -> int {
  cxxopts::Options options("ZRegex", "JIT Compiled LLVM Regex Engine");
  options.add_options()("r,regex", "Regex", cxxopts::value<std::string>())(
      "s,string", "String to match the pattern against", cxxopts::value<std::string>())(
      "d,debug", "Enable debugging");

  auto result = options.parse(argc, argv);
  auto debug_mode = result["debug"].as<bool>();
  spdlog::info("Debugging Enabled : {}", debug_mode);
  LLVMCodeGen llvm(debug_mode);
  auto rgx = result["regex"].as<std::string>();
  auto inp = result["string"].as<std::string>();
  llvm.compile(rgx);
  llvm.run(inp);
  return 0;
}
