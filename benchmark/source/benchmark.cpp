//
// Created by Mohamed El Zarei on 21/12/2021.
//

#include <benchmark/benchmark.h>
#include <codegen/codegen.h>
#include <re2/re2.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <regex>
#include <sstream>

#include "spdlog/cfg/argv.h"
#include "spdlog/pattern_formatter.h"

#define DFA_LLVM 0
#define DFA_CPP 1
#define RE2_TYPE 2
#define STDRGX 3

#define INIT                                        \
  std::ifstream st(dataset, std::ios_base::binary); \
  double matches = 0;

#define LABEL                                 \
  if (state.range(0) == DFA_LLVM) {           \
    state.SetLabel("DFA LLVM");               \
  } else if (state.range(0) == RE2_TYPE) {    \
    state.SetLabel("RE2");                    \
  } else if (state.range(0) == STDRGX) {      \
    state.SetLabel("Standard Library Regex"); \
  } else if (state.range(0) == DFA_CPP) {     \
    state.SetLabel("DFA CPP");                \
  }

#define LOAD_DATA             \
  std::string content;        \
  std::string line;           \
  while (getline(st, line)) { \
    content.append(line);     \
  }                           \
  content.append(append);

#define APPLY_PARTIAL                                                                \
  std::string result;                                                                \
  if (partial && state.range(0) == STDRGX) {                                         \
    result = ".*" + pattern + ".*";                                                  \
  } else if (partial && (state.range(0) == DFA_LLVM || state.range(0) == DFA_CPP)) { \
    result = "%" + pattern;                                                          \
  } else {                                                                           \
    result = pattern;                                                                \
  }
static void BENCH_ALL_ASCII(benchmark::State& state, const std::string& pattern,
                            const std::string&& append, const bool&& partial,
                            const std::string& dataset) {
  INIT LABEL LOAD_DATA APPLY_PARTIAL;
  //  spdlog::info("Pattern {}", result);

  if (state.range(0) == STDRGX) {
    std::regex regex(result);
    for (auto _ : state) {
      benchmark::DoNotOptimize(matches = std::regex_match(content, regex));
    }
  } else if (state.range(0) == RE2_TYPE) {
    re2::RE2 p(result, re2::RE2::Latin1);
    if (partial) {
      for (auto _ : state) {
        benchmark::DoNotOptimize(matches = re2::RE2::PartialMatch(content, p));
      }
    } else {
      for (auto _ : state) {
        benchmark::DoNotOptimize(matches = re2::RE2::FullMatch(content, p));
      }
    }

  } else {
    auto backend_type = state.range(0) == DFA_LLVM ? ZRegex::CodegenBackendType::LLVM
                                                   : ZRegex::CodegenBackendType::CPP;
    auto encoding = ZRegex::Encoding::ASCII;
    ZRegex::Codegen code_generator(backend_type, encoding);
    for (auto _ : state) {
      code_generator.Compile(result.c_str());
      benchmark::DoNotOptimize(matches = code_generator.Run(content.c_str()));
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_ALL_UTF8(benchmark::State& state, const std::string& pattern,
                           const std::string&& append, const bool&& partial,
                           const std::string& dataset) {
  INIT LABEL LOAD_DATA APPLY_PARTIAL;
  //  spdlog::info("Pattern {}", result);

  if (state.range(0) == STDRGX) {
    std::regex regex(result);
    for (auto _ : state) {
      benchmark::DoNotOptimize(matches = std::regex_match(content, regex));
    }
  } else if (state.range(0) == RE2_TYPE) {
    re2::RE2 p(result);
    if (partial) {
      for (auto _ : state) {
        benchmark::DoNotOptimize(matches = re2::RE2::PartialMatch(content, p));
      }
    } else {
      for (auto _ : state) {
        benchmark::DoNotOptimize(matches = re2::RE2::FullMatch(content, p));
      }
    }

  } else {
    auto backend_type = state.range(0) == DFA_LLVM ? ZRegex::CodegenBackendType::LLVM
                                                   : ZRegex::CodegenBackendType::CPP;
    auto encoding = ZRegex::Encoding::UTF8;
    //    spdlog::info("Backend Type: {}, Encoding {}", backend_type, encoding);
    ZRegex::Codegen code_generator(backend_type, encoding);
    for (auto _ : state) {
      code_generator.Compile(result.c_str());
      benchmark::DoNotOptimize(matches = code_generator.Run(content.c_str()));
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_LINE(benchmark::State& state, const std::string& pattern, const bool&& partial,
                       ZRegex::Encoding&& encoding, const std::string& dataset) {
  INIT LABEL APPLY_PARTIAL;
  std::string line;
  if (state.range(0) == RE2_TYPE) {
    auto re2_enc = encoding == ZRegex::Encoding::UTF8 ? re2::RE2::DefaultOptions : re2::RE2::Latin1;
    if (partial) {
      for (auto _ : state) {
        re2::RE2 p(result, re2_enc);
        while (getline(st, line)) {
          benchmark::DoNotOptimize(matches += re2::RE2::PartialMatch(line, p));
        }
      }
    }
  } else if (state.range(0) == STDRGX) {
    spdlog::info("Pattern {}", result);
    for (auto _ : state) {
      std::regex regex(result);
      while (getline(st, line)) {
        benchmark::DoNotOptimize(matches += std::regex_match(line, regex));
      }
    }

  } else {
    auto backend_type = state.range(0) == DFA_LLVM ? ZRegex::CodegenBackendType::LLVM
                                                   : ZRegex::CodegenBackendType::CPP;
    ZRegex::Codegen code_generator(backend_type, encoding);
    for (auto _ : state) {
      code_generator.Compile(result.c_str());
      while (getline(st, line)) {
        benchmark::DoNotOptimize(matches += code_generator.Run(line.c_str()));
      }
    }
  }

  state.counters["Matches"] = matches;
}
BENCHMARK_CAPTURE(BENCH_LINE, u8"Unicode, p=ä", u8"ä" /*pattern*/, true /*partial*/,
                  ZRegex::Encoding::UTF8 /*encoding*/,
                  "/Users/melzarei/Desktop/bench_data/unicode_100mb.txt")
    ->DenseRange(DFA_LLVM, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BENCH_LINE, u8"Unicode, p=ä{1,4}", u8"ä{1,4}" /*pattern*/, true /*partial*/,
                  ZRegex::Encoding::UTF8 /*encoding*/,
                  "/Users/melzarei/Desktop/bench_data/unicode_100mb.txt")
    ->DenseRange(DFA_LLVM, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BENCH_LINE, u8"Unicode, p=([äTȧ]){2,5}", u8"([äTȧ]){2,5}" /*pattern*/,
                  true /*partial*/, ZRegex::Encoding::UTF8 /*encoding*/,
                  "/Users/melzarei/Desktop/bench_data/unicode_100mb.txt")
    ->DenseRange(DFA_LLVM, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);

int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  spdlog::cfg::load_argv_levels(argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}