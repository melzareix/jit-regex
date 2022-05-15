#include <codegen/codegen.h>

#include <string>

#include "cxxopts.hpp"
#include "fa/special/kmp.h"
#include "fa/special/simd.h"
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
      "x,bytedfa", "UTF-8 ByteDFA encoding", cxxopts::value<bool>()->default_value("false"))(
      "h,help", "Print help");
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

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
  // std::string p = "ab";
  // std::string sx = "aaaaaaaaaaaaaaaaaaaaaaxlaaaaaaaaaaaaaaaaaaaaaaabx";

  // auto haystack = sx.c_str(), needle = p.c_str();
  // auto barr = ZRegex::SIMDSubstringMatch::preprocess(needle, p.size());
  // auto r = ZRegex::SIMDSubstringMatch::epsm_a(needle, p.size(), haystack, sx.size(), barr);
  // ZRegex::SIMDSubstringMatch::clean_b(barr, p.size());
  // ZRegex::SIMDSubstringMatch::sse4_strstr_anysize(sx, 10, p, 5);

  auto needle = rgx.c_str();
  auto barr = ZRegex::SIMDSubstringMatch::preprocess(needle, rgx.size());
  while (getline(std::cin, s)) {
    auto z = s.c_str();
    spdlog::debug("Run Result : {}", code_generator.Run(z));
    auto r = ZRegex::SIMDSubstringMatch::epsm_a(needle, rgx.size(), z, s.size(), barr);
    // auto r = ZRegex::SIMDSubstringMatch::epsm_a_alternative(needle, rgx.size(), z, s.size(),
    // barr);
    auto rr = code_generator.Run(z);
    spdlog::info("Results: {} - {}", r, rr);
  }

  return 0;
}
