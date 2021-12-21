#include <automaton/Automaton.h>
#include <codegen/llvmCodegen.h>
#include <re2/re2.h>

#include <chrono>
#include <cxxopts.hpp>
#include <fstream>
#include <regex>
#include <string>

#include "spdlog/cfg/env.h"
#include <benchmark/benchmark.h>

using namespace std::chrono;

void benchmark_dfa(std::string& benchmark, LLVMCodeGen& codegen) {
  // run in benchmark mode
  spdlog::info("DFA Results");
  std::ifstream st(benchmark, std::ios_base::binary);
  uint64_t total_dfa_time = 0;
  int totalNumOfLines = 0;
  int matched = 0;
  std::string line;
  auto total_time_start = high_resolution_clock::now();
  while (std::getline(st, line)) {
    totalNumOfLines++;
    auto start = high_resolution_clock::now();
    bool m = codegen.run(line);
    auto end = high_resolution_clock::now();
    matched += m;
    total_dfa_time += (duration_cast<microseconds>(end - start)).count();
  }
  auto total_time_end = high_resolution_clock::now();
  spdlog::info("Number of lines matched: {} out of {}.", matched, totalNumOfLines);
  spdlog::info("Matching Execution time: {} microseconds.", total_dfa_time);
  auto totalTime = (duration_cast<microseconds>(total_time_end - total_time_start).count());
  spdlog::info("Total Loop Execution Time: {} seconds.", ((double)(totalTime) / (1e6)));
}

void benchmark_re2(std::string& benchmark, re2::RE2& rgx) {
  // run in benchmark mode
  spdlog::info("RE2 Results");
  std::ifstream st(benchmark, std::ios_base::binary);
  uint64_t total_dfa_time = 0;
  int totalNumOfLines = 0;
  int matched = 0;
  std::string line;
  auto total_time_start = high_resolution_clock::now();
  while (std::getline(st, line)) {
    totalNumOfLines++;
    auto start = high_resolution_clock::now();
    bool m = re2::RE2::PartialMatch(line, rgx);
    auto end = high_resolution_clock::now();
    matched += m;
    total_dfa_time += (duration_cast<microseconds>(end - start)).count();
  }
  auto total_time_end = high_resolution_clock::now();
  spdlog::info("Number of lines matched: {} out of {}.", matched, totalNumOfLines);
  spdlog::info("Matching Execution time: {} microseconds.", total_dfa_time);
  auto totalTime = (duration_cast<microseconds>(total_time_end - total_time_start).count());
  spdlog::info("Total Loop Execution Time: {} seconds.", ((double)(totalTime) / (1e6)));
}

void benchmark_std(std::string& benchmark, std::regex& rgx) {
  // run in benchmark mode
  spdlog::info("std::regex Results");
  std::ifstream st(benchmark, std::ios_base::binary);
  uint64_t total_dfa_time = 0;
  int totalNumOfLines = 0;
  int matched = 0;
  std::string line;
  auto total_time_start = high_resolution_clock::now();
  while (std::getline(st, line)) {
    totalNumOfLines++;
    auto start = high_resolution_clock::now();
    bool m = std::regex_search(line, rgx);
    auto end = high_resolution_clock::now();
    matched += m;
    total_dfa_time += (duration_cast<microseconds>(end - start)).count();
  }
  auto total_time_end = high_resolution_clock::now();
  spdlog::info("Number of lines matched: {} out of {}.", matched, totalNumOfLines);
  spdlog::info("Matching Execution time: {} microseconds.", total_dfa_time);
  auto totalTime = (duration_cast<microseconds>(total_time_end - total_time_start).count());
  spdlog::info("Total Loop Execution Time: {} seconds.", ((double)(totalTime) / (1e6)));
}

auto main(int argc, char** argv) -> int {
  spdlog::cfg::load_env_levels();

  cxxopts::Options options("ZRegex", "JIT Compiled LLVM Regex Engine");
  options.add_options()("r,regex", "Regex", cxxopts::value<std::string>())(
      "s,string", "String to match the pattern against", cxxopts::value<std::string>())(
      "b,benchmark", "Benchmark Dataset", cxxopts::value<std::string>()->default_value(""));

  auto result = options.parse(argc, argv);

  LLVMCodeGen llvm;
  auto rgx = result["regex"].as<std::string>();
  auto pattern = fmt::format("/Users/melzarei/{}.json", rgx);

  std::ifstream ifs(pattern);

  auto a = Automaton::from_json(ifs);
  llvm.compile(a);
  re2::RE2 re2rgx(rgx);
  std::regex stdrgx(rgx, std::regex_constants::icase);

  auto benchmark = result["benchmark"].as<std::string>();
  if (!benchmark.empty()) {
    benchmark_dfa(benchmark, llvm);
    benchmark_re2(benchmark, re2rgx);
    benchmark_std(benchmark, stdrgx);
  } else {
    auto inp = result["string"].as<std::string>();
    llvm.run(inp);
  }

  return 0;
}
