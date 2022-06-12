#include <codegen/codegen.h>

#include <string>

#include "Engine.h"
#include "cxxopts.hpp"
#include "fa/special/kmp.h"
#include "fa/special/simd.h"
#include "helpers/utf8.h"
#include "helpers/utf8range.h"
#include "spdlog/cfg/argv.h"
#include "spdlog/spdlog.h"
using namespace ZRegex;

CodegenOpts::OptimizationLevel parseOptimizationLevel(std::string& level) {
  if (level == "O0") return CodegenOpts::OptimizationLevel::O0;
  if (level == "O1") return CodegenOpts::OptimizationLevel::O1;
  if (level == "O2") return CodegenOpts::OptimizationLevel::O2;
  if (level == "O3") return CodegenOpts::OptimizationLevel::O3;
  if (level == "Oz") return CodegenOpts::OptimizationLevel::Oz;
  if (level == "Os") return CodegenOpts::OptimizationLevel::Os;
  spdlog::warn("Wrong optimization level defaulting to O2.");
  return CodegenOpts::OptimizationLevel::O2;
}
auto main(int argc, char** argv) -> int {
  spdlog::cfg::load_argv_levels(argc, argv);

  cxxopts::Options options("ZRegex", "JIT Compiled Regex Engine");
  options.add_options()("r,regex", "Regex", cxxopts::value<std::string>())(
      "b,backend", "Backend", cxxopts::value<std::string>())(
      "a,ascii", "Support utf-8 encoding", cxxopts::value<bool>()->default_value("false"))(
      "x,bytedfa", "UTF-8 ByteDFA encoding", cxxopts::value<bool>()->default_value("false"))(
      "o,optimizationlevel", "Backend Optimization Level",
      cxxopts::value<std::string>()->default_value("O2"))("h,help", "Print help");
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  auto rgx = result["regex"].as<std::string>();
  auto backend = result["backend"].as<std::string>();
  auto optLevelstr = result["optimizationlevel"].as<std::string>();
  auto bytedfa = result["bytedfa"].as<bool>();

  auto isAscii = result["ascii"].as<bool>();
  auto encoding = isAscii ? CodegenOpts::DFAEncoding::ASCII : CodegenOpts::DFAEncoding::UTF8;
  auto backend_type = backend == "cpp" ? CodegenOpts::CodegenBackendType::CPP
                                       : CodegenOpts::CodegenBackendType::LLVM;
  auto optimizationLevel = parseOptimizationLevel(optLevelstr);

  auto opts = CodegenOpts(backend_type, encoding, optimizationLevel, bytedfa);
  spdlog::warn(
      "Compiling Pattern {} - Options [Backend={}, Unicode={}, byteDFA={}, optimizationLevel={}]",
      rgx, backend, !isAscii, bytedfa, opts.getOptimizationLevelString());

  Engine engine(rgx, opts);
  spdlog::warn("Pattern Compiled {}", rgx);

  std::string s;
  while (getline(std::cin, s)) {
    auto z = s.c_str();
    auto rr = engine.Run(z);
    spdlog::info("Matching Result: {}", rr);
  }

  return 0;
}
