#include <codegen/codegen.h>

#include <string>

#include "cxxopts.hpp"
#include "helpers/utf8.h"
#include "helpers/utf8range.h"
#include "spdlog/cfg/argv.h"
#include "spdlog/spdlog.h"

using namespace ZRegex;

auto main(int argc, char** argv) -> int {
  spdlog::cfg::load_argv_levels(argc, argv);

  cxxopts::Options options("ZRegex", "JIT Compiled Regex Engine");
  options.add_options()("r,regex", "Regex", cxxopts::value<std::string>())(
      "b,backend", "Backend", cxxopts::value<std::string>())(
      "a,ascii", "Support utf-8 encoding", cxxopts::value<bool>()->default_value("false"))(
      "x,bytedfa", "UTF-8 ByteDFA encoding", cxxopts::value<bool>()->default_value("false"));
  auto result = options.parse(argc, argv);
  auto rgx = result["regex"].as<std::string>();
  auto backend = result["backend"].as<std::string>();
  auto bytedfa = result["bytedfa"].as<bool>();

  auto isAscii = result["ascii"].as<bool>();
  auto encoding = isAscii ? CodegenOpts::DFAEncoding::ASCII : CodegenOpts::DFAEncoding::UTF8;
  auto backend_type = backend == "cpp" ? CodegenOpts::CodegenBackendType::CPP
                                       : CodegenOpts::CodegenBackendType::LLVM;
  auto optimizationLevel = CodegenOpts::OptimizationLevel::O3;

  auto opts = CodegenOpts(backend_type, encoding, optimizationLevel, bytedfa);
  spdlog::warn("Compiling Pattern {} - Options [Backend={}, Unicode={}, byteDFA={}]", rgx, backend,
               !isAscii, bytedfa);
  ZRegex::Codegen code_generator(opts);
  code_generator.Compile(rgx.c_str());
  spdlog::warn("Pattern Compiled {}", rgx);

  std::string s;
  while (getline(std::cin, s)) {
    auto z = s.c_str();
    spdlog::debug("Run Result : {}", code_generator.Run(z));
  }

  return 0;
}
