#include <codegen/codegen.h>

#include <string>

#include "cxxopts.hpp"
#include "spdlog/cfg/env.h"
#include "spdlog/spdlog.h"

auto main(int argc, char** argv) -> int {
  spdlog::cfg::load_env_levels();

  cxxopts::Options options("ZRegex", "JIT Compiled Regex Engine");
  options.add_options()("r,regex", "Regex", cxxopts::value<std::string>())(
      "b,backend", "Backend", cxxopts::value<std::string>());
  auto result = options.parse(argc, argv);
  auto rgx = result["regex"].as<std::string>();
  auto backend = result["backend"].as<std::string>();

  ZRegex::Codegen code_generator(backend == "cpp" ? ZRegex::CodegenBackendType::CPP
                                                  : ZRegex::CodegenBackendType::LLVM);
  code_generator.Compile(rgx.c_str());
  spdlog::warn("Pattern Compiled {}", rgx);

  std::string s;
  while(std::cin >> s) {
    spdlog::info("Run Result : {}", code_generator.Run(s.c_str()));
  }

  return 0;
}
