//
// Created by Mohamed El Zarei on 21/12/2021.
//

#include <benchmark/benchmark.h>
#include <codegen/codegen.h>
#include <re2/re2.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <fstream>
#include <functional>
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
    code_generator.Compile(result.c_str());
    for (auto _ : state) {
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
    code_generator.Compile(result.c_str());
    for (auto _ : state) {
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
    re2::RE2 p(result, re2_enc);
    if (partial) {
      for (auto _ : state) {
        while (getline(st, line)) {
          benchmark::DoNotOptimize(matches += re2::RE2::PartialMatch(line, p));
        }
      }
    }
  } else if (state.range(0) == STDRGX) {
    spdlog::info("Pattern {}", result);
    std::regex regex(result);
    for (auto _ : state) {
      while (getline(st, line)) {
        benchmark::DoNotOptimize(matches += std::regex_match(line, regex));
      }
    }

  } else {
    auto backend_type = state.range(0) == DFA_LLVM ? ZRegex::CodegenBackendType::LLVM
                                                   : ZRegex::CodegenBackendType::CPP;
    ZRegex::Codegen code_generator(backend_type, encoding);
    code_generator.Compile(result.c_str());
    for (auto _ : state) {
      while (getline(st, line)) {
        benchmark::DoNotOptimize(matches += code_generator.Run(line.c_str()));
      }
    }
  }

  state.counters["Matches"] = matches;
}

// DFA ASCII vs UTF8 Benchmarks
// (1) 100 mb all in one
 BENCHMARK_CAPTURE(BENCH_ALL_UTF8, "Unicode Matcher, Ascii p/t 100MB, p=1", "1" /*pattern*/,
                  "1x" /*append*/, true /*partial*/,
                  "/Users/melzarei/Desktop/bench_data/ascii_100mb.txt")
    ->DenseRange(DFA_LLVM, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);

 BENCHMARK_CAPTURE(BENCH_ALL_ASCII, "ASCII Everything 100MB, p=1", "1" /*pattern*/,
                  "1x" /*append*/, true /*partial*/,
                  "/Users/melzarei/Desktop/bench_data/ascii_100mb.txt")
    ->DenseRange(DFA_LLVM, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);

// (2) 100 mb line by line
BENCHMARK_CAPTURE(BENCH_LINE, "Unicode Matcher, Ascii p/t 100MB, p=Y", "Y" /*pattern*/,
                  true /*partial*/, ZRegex::Encoding::UTF8,
                  "/Users/melzarei/Desktop/bench_data/ascii_100mb.txt")
    ->DenseRange(DFA_LLVM, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_LINE, "ASCII Everything 100MB, p=Y", "Y" /*pattern*/, true /*partial*/,
                  ZRegex::Encoding::ASCII /*encoding*/,
                  "/Users/melzarei/Desktop/bench_data/ascii_100mb.txt")
    ->DenseRange(DFA_LLVM, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);

// Unicode Benchmarks
// BENCHMARK_CAPTURE(BENCH_ALL_UTF8, "UNICODE 100MB, p=💙", u8"💙" /*pattern*/, u8"💙x" /*append*/,
//                  true /*partial*/, "/Users/melzarei/Desktop/bench_data/unicode_100mb.txt")
//    ->DenseRange(DFA_LLVM, RE2_TYPE)
//    ->Unit(benchmark::kMillisecond);
//
// BENCHMARK_CAPTURE(BENCH_ALL_UTF8, "UNICODE 100MB p=💙{3,5}", u8"💙{3,5}" /*pattern*/,
//                  u8"💙💙💙💙1" /*append*/, true /*partial*/,
//                  "/Users/melzarei/Desktop/bench_data/unicode_100mb.txt")
//    ->DenseRange(DFA_LLVM, RE2_TYPE)
//    ->Unit(benchmark::kMillisecond);
//
//// ASCII Benchmarks
// BENCHMARK_CAPTURE(BENCH_ALL_ASCII, "ASCII 100MB p=1", "1" /*pattern*/, "1x" /*append*/,
//                  true /*partial*/, "/Users/melzarei/Desktop/bench_data/ascii_100mb.txt")
//    ->DenseRange(DFA_LLVM, RE2_TYPE)
//    ->Unit(benchmark::kMillisecond);
//
// BENCHMARK_CAPTURE(BENCH_ALL_ASCII, "ASCII 10MB p=1{3,5}", "1{3,5}" /*pattern*/, "1111x"
// /*append*/,
//                  true /*partial*/, "/Users/melzarei/Desktop/bench_data/ascii_100mb.txt")
//    ->DenseRange(DFA_LLVM, RE2_TYPE)
//    ->Unit(benchmark::kMillisecond);

 int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  spdlog::cfg::load_argv_levels(argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}