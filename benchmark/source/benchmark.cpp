//
// Created by Mohamed El Zarei on 21/12/2021.
//

#include <automaton/Automaton.h>
#include <benchmark/benchmark.h>
#include <codegen/llvmCodegen.h>
#include <re2/re2.h>

#include <cstdint>
#include <fstream>
#include <functional>
#include <regex>

#define DFA_TYPE 0
#define RE2_TYPE 1
#define STDLIB_TYPE 2

static void BM_DATASET(benchmark::State& state, std::string pattern, std::string dataset) {
  int matches = 0;
  std::string line;
  std::function<void()> fn;

  if (state.range(0) == DFA_TYPE) {
    state.SetLabel("DFA");
    auto llvm = std::make_shared<LLVMCodeGen>();
    auto dfa_json = fmt::format("/Users/melzarei/{}.json", pattern);
    std::ifstream ifs(dfa_json);
    auto a = Automaton::from_json(ifs);
    llvm->compile(std::move(a));
    fn = [&matches, llvm, &line]() { benchmark::DoNotOptimize(matches += llvm->run(line)); };
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

BENCHMARK_CAPTURE(BM_DATASET, LOREM_IPSUM, "L.*ipsum", "/Users/melzarei/Desktop/a.txt")
    ->DenseRange(DFA_TYPE, RE2_TYPE);
BENCHMARK_CAPTURE(BM_DATASET, ab, "ab", "/Users/melzarei/Desktop/a.txt")
    ->DenseRange(DFA_TYPE, RE2_TYPE);

BENCHMARK_MAIN();