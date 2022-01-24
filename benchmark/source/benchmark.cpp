//
// Created by Mohamed El Zarei on 21/12/2021.
//

#include <automaton/Automaton.h>
#include <benchmark/benchmark.h>
#include <codegen/codegen.h>
#include <re2/re2.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <functional>
#include <regex>

#include "spdlog/pattern_formatter.h"

#define DFA_LLVM_TYPE 0
#define DFA_CPP_TYPE 1
#define RE2_TYPE 2
#define STDLIB_TYPE 3

static void BM_DATASET(benchmark::State& state, const std::string& pattern,
                       const std::string& dataset) {
  double matches = 0;
  std::string line;
  std::function<void()> fn;

  if (state.range(0) == DFA_LLVM_TYPE) {
    state.SetLabel("DFA LLVM");
    auto llvm = std::make_shared<ZRegex::Codegen>(ZRegex::CodegenBackendType::LLVM);
    auto dfa_json = fmt::format("/Users/melzarei/{}.json", pattern);
    llvm->Compile(dfa_json.c_str());
    fn = [&matches, llvm, &line]() {
      benchmark::DoNotOptimize(matches += llvm->Run(line.c_str()));
    };
  } else if (state.range(0) == DFA_CPP_TYPE) {
    state.SetLabel("DFA CPP");
    auto llvm = std::make_shared<ZRegex::Codegen>(ZRegex::CodegenBackendType::CPP);
    auto dfa_json = fmt::format("/Users/melzarei/{}.json", pattern);
    llvm->Compile(dfa_json.c_str());
    fn = [&matches, llvm, &line]() {
      benchmark::DoNotOptimize(matches += llvm->Run(line.c_str()));
    };
  } else if (state.range(0) == RE2_TYPE) {
    state.SetLabel("RE2");
    auto re2pattern = std::make_shared<re2::RE2>(pattern);
    fn = [&matches, re2pattern, &line]() {
      benchmark::DoNotOptimize(matches += re2::RE2::PartialMatch(line, *re2pattern));
    };
  } else {
    state.SetLabel("STD::REGEX");
    auto stdrgx = std::make_shared<std::regex>(pattern);
    fn = [&matches, stdrgx, &line]() {
      benchmark::DoNotOptimize(matches += std::regex_search(line, *stdrgx));
    };
  }

  std::ifstream st(dataset, std::ios_base::binary);
  for (auto _ : state) {
    while (std::getline(st, line)) {
      fn();
    }
  }
  state.counters["Matches"] = matches;
}

BENCHMARK_CAPTURE(BM_DATASET, TWAIN_COUNT, "ange", "/Users/melzarei/Desktop/ds.txt")
    ->DenseRange(DFA_LLVM_TYPE, RE2_TYPE);

BENCHMARK_MAIN();