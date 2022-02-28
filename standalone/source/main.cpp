#include <codegen/codegen.h>

#include <string>

#include "cxxopts.hpp"
#include "helpers/utf8.h"
#include "helpers/utf8range.h"
#include "spdlog/cfg/argv.h"
#include "spdlog/spdlog.h"

using namespace ZRegex;

std::vector<uint8_t> sequential_data(size_t size) {
  std::vector<uint8_t> ret(size);

  for (size_t i = 0; i < size; ++i) {
    ret[i] = (uint8_t)(i & size_t(255));
  }

  return ret;
}

auto main(int argc, char** argv) -> int {
  spdlog::cfg::load_argv_levels(argc, argv);

  cxxopts::Options options("ZRegex", "JIT Compiled Regex Engine");
  options.add_options()("r,regex", "Regex", cxxopts::value<std::string>())(
      "b,backend", "Backend", cxxopts::value<std::string>())(
      "a,ascii", "Support utf-8 encoding", cxxopts::value<bool>()->default_value("false"));
  auto result = options.parse(argc, argv);
  auto rgx = result["regex"].as<std::string>();
  auto backend = result["backend"].as<std::string>();

  auto isAscii = result["ascii"].as<bool>();
  auto encoding = isAscii ? CodegenOpts::DFAEncoding::ASCII : CodegenOpts::DFAEncoding::UTF8;
  auto backend_type = backend == "cpp" ? CodegenOpts::CodegenBackendType::CPP
                                       : CodegenOpts::CodegenBackendType::LLVM;
  auto optimizationLevel = CodegenOpts::OptimizationLevel::O3;
  auto sx = std::string(u8"\u0000");
  auto ex = std::string(u8"\uffff");
  auto r = UTF8Range(sx, ex);
  r.PrintRange();
  while (r.hasNext()) {
    r.PrintRange();
  }
  auto opts = CodegenOpts(backend_type, encoding, optimizationLevel);
  spdlog::warn("Compiling Pattern {} - Options [Backend={}, Unicode={}]", rgx, backend, !isAscii);
  ZRegex::Codegen code_generator(opts);
  code_generator.Compile(rgx.c_str());
  spdlog::warn("Pattern Compiled {}", rgx);
  auto d = sequential_data(1024);

  std::string s;
  while (getline(std::cin, s)) {
    auto z = s.c_str();
    spdlog::info("Run Result : {}", code_generator.Run(z));
  }

  return 0;
}
