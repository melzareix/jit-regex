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

#include "fa/special/kmp.h"
#include "spdlog/cfg/argv.h"
#include "spdlog/pattern_formatter.h"

#define DFA_LLVM_U8 0
#define DFA_LLVM_U32 1
#define DFA_CPP_U8 2
#define DFA_CPP_U32 3
#define RE2_TYPE 4
#define KMP_TYPE 5
#define STDRGX 6

#define INIT                                        \
  std::ifstream st(dataset, std::ios_base::binary); \
  double matches = 0;

#define LABEL                                  \
  if (state.range(0) == DFA_LLVM_U8) {         \
    state.SetLabel("DFA LLVM U8");             \
  } else if (state.range(0) == DFA_LLVM_U32) { \
    state.SetLabel("DFA LLVM U32");            \
  } else if (state.range(0) == DFA_CPP_U8) {   \
    state.SetLabel("DFA CPP U8");              \
  } else if (state.range(0) == DFA_CPP_U32) {  \
    state.SetLabel("DFA CPP U32");             \
  } else if (state.range(0) == RE2_TYPE) {     \
    state.SetLabel("RE2");                     \
  } else if (state.range(0) == KMP_TYPE) {     \
    state.SetLabel("KMP Interpretted");        \
  } else if (state.range(0) == STDRGX) {       \
    state.SetLabel("Standard Library Regex");  \
  }

#define LOAD_DATA             \
  std::string content;        \
  std::string line;           \
  while (getline(st, line)) { \
    content.append(line);     \
  }                           \
  content.append(append);

#define APPLY_PARTIAL                                                                       \
  std::string result;                                                                       \
  if (partial && state.range(0) == STDRGX) {                                                \
    result = ".*(" + pattern + ").*";                                                       \
  } else if (partial && (state.range(0) >= DFA_LLVM_U8 && state.range(0) <= DFA_CPP_U32)) { \
    result = "%(" + pattern + ")";                                                          \
  } else {                                                                                  \
    result = pattern;                                                                       \
  }
// static void BENCH_ALL_ASCII(benchmark::State& state, const std::string& pattern,
//                             const std::string&& append, const bool&& partial,
//                             const std::string& dataset) {
//   INIT LABEL LOAD_DATA APPLY_PARTIAL;
//   //  spdlog::info("Pattern {}", result);

//   if (state.range(0) == STDRGX) {
//     std::regex regex(result);
//     for (auto _ : state) {
//       benchmark::DoNotOptimize(matches = std::regex_match(content, regex));
//     }
//   } else if (state.range(0) == RE2_TYPE) {
//     re2::RE2 p(result, re2::RE2::Latin1);
//     if (partial) {
//       for (auto _ : state) {
//         benchmark::DoNotOptimize(matches = re2::RE2::PartialMatch(content, p));
//       }
//     } else {
//       for (auto _ : state) {
//         benchmark::DoNotOptimize(matches = re2::RE2::FullMatch(content, p));
//       }
//     }

//   } else {
//     auto backend_type = state.range(0) == DFA_LLVM ?
//     ZRegex::CodegenOpts::CodegenBackendType::LLVM
//                                                    :
//                                                    ZRegex::CodegenOpts::CodegenBackendType::CPP;
//     auto encoding = ZRegex::Encoding::ASCII;
//     ZRegex::Codegen code_generator(backend_type, encoding);
//     for (auto _ : state) {
//       code_generator.Compile(result.c_str());
//       benchmark::DoNotOptimize(matches = code_generator.Run(content.c_str()));
//     }
//   }
//   state.counters["Matches"] = matches;
// }

static void BENCH_ALL(benchmark::State& state, const std::string& pattern, const bool&& partial,
                      ZRegex::CodegenOpts&& opts, const std::string&& append,
                      const std::string& dataset) {
  INIT LABEL LOAD_DATA APPLY_PARTIAL;

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

  } else if (state.range(0) == KMP_TYPE) {
    ZRegex::KMPAlgorithm kmp(pattern);
    for (auto _ : state) {
      benchmark::DoNotOptimize(matches = kmp.Search(content));
    }

  } else {
    if (state.range(0) == DFA_LLVM_U8) {
      opts.SetByteDFA(true);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::LLVM);
    } else if (state.range(0) == DFA_LLVM_U32) {
      opts.SetByteDFA(false);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::LLVM);
    } else if (state.range(0) == DFA_CPP_U32) {
      opts.SetByteDFA(false);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::CPP);
    } else if (state.range(0) == DFA_CPP_U8) {
      opts.SetByteDFA(true);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::CPP);
    }
    ZRegex::Codegen code_generator(opts);

    for (auto _ : state) {
      code_generator.Compile(result.c_str());
      benchmark::DoNotOptimize(matches = code_generator.Run(content.c_str()));
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_LINE(benchmark::State& state, const std::string& pattern, const bool&& partial,
                       ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT LABEL APPLY_PARTIAL;
  std::string line;
  if (state.range(0) == RE2_TYPE) {
    auto re2_enc = (opts.IsUTF8() || opts.IsUTF32()) ? re2::RE2::DefaultOptions : re2::RE2::Latin1;
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

  } else if (state.range(0) == KMP_TYPE) {
    ZRegex::KMPAlgorithm kmp(pattern);
    for (auto _ : state) {
      while (getline(st, line)) {
        benchmark::DoNotOptimize(matches += kmp.Search(line));
      }
    }
  } else {
    if (state.range(0) == DFA_LLVM_U8) {
      opts.SetByteDFA(true);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::LLVM);
    } else if (state.range(0) == DFA_LLVM_U32) {
      opts.SetByteDFA(false);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::LLVM);
    } else if (state.range(0) == DFA_CPP_U32) {
      opts.SetByteDFA(false);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::CPP);
    } else if (state.range(0) == DFA_CPP_U8) {
      opts.SetByteDFA(true);
      opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
      opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::CPP);
    }
    ZRegex::Codegen code_generator(opts);
    for (auto _ : state) {
      code_generator.Compile(result.c_str());
      while (getline(st, line)) {
        benchmark::DoNotOptimize(matches += code_generator.Run(line.c_str()));
      }
    }
  }

  state.counters["Matches"] = matches;
}
// BENCHMARK_CAPTURE(BENCH_LINE, u8"UTF8, p=Twain|twain", u8"Twain|twain" /*pattern*/,
//                   true /*partial*/, ZRegex::CodegenOpts() /*encoding*/,
//                   "/home/elzarei/bench_data/3200.txt")
//     ->DenseRange(DFA_LLVM_U8, RE2_TYPE);
// BENCHMARK_CAPTURE(BENCH_LINE, u8"UTF8, p=[a-zA-Z0-9\\_]+nn", u8"[a-zA-Z0-9\\_]+nn" /*pattern*/,
//                   true /*partial*/, ZRegex::CodegenOpts() /*encoding*/,
//                   "/home/elzarei/bench_data/3200.txt")
//     ->DenseRange(DFA_LLVM_U8, RE2_TYPE);

// BENCHMARK_CAPTURE(BENCH_ALL, u8"UTF8, p=💙", u8"💙" /*pattern*/, true /*partial*/,
//                   ZRegex::CodegenOpts() /*encoding*/, u8"💙" /*append*/,
//                   "/home/elzarei/bench_data/unicode_100mb.txt")
//     ->DenseRange(DFA_LLVM_U8, KMP_TYPE)
//     ->Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(BENCH_ALL, u8"UTF8, p=💙{2,5}", u8"💙{2,5}" /*pattern*/, true /*partial*/,
//                   ZRegex::CodegenOpts() /*encoding*/, u8"💙💙💙💙" /*append*/,
//                   "/home/elzarei/bench_data/unicode_100mb.txt")
//     ->DenseRange(DFA_LLVM_U8, RE2_TYPE)
//     ->Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(BENCH_LINE, u8"UTF8, p=ä{2,3}", u8"ä" /*pattern*/, true /*partial*/,
//                   ZRegex::CodegenOpts() /*encoding*/,
//                   "/home/elzarei/bench_data/unicode_100mb.txt")
//     ->DenseRange(DFA_LLVM_U8, KMP_TYPE);

// BENCHMARK_CAPTURE(BENCH_LINE, u8"Unicode, p=ä{1,4}", u8"ä{1,4}" /*pattern*/, true /*partial*/,
//                   ZRegex::CodegenOpts() /*encoding*/,
//                   "/home/elzarei/bench_data/unicode_100mb.txt")
//     ->DenseRange(DFA_LLVM_U8, RE2_TYPE);
// BENCHMARK_CAPTURE(BENCH_LINE, u8"Unicode, p=([äTȧ]){2,5}", u8"([äTȧ]){2,5}" /*pattern*/,
//                   true /*partial*/, ZRegex::CodegenOpts() /*encoding*/,
//                   "/home/elzarei/bench_data/unicode_100mb.txt")
//     ->DenseRange(DFA_LLVM_U8, RE2_TYPE);

BENCHMARK_CAPTURE(BENCH_LINE, u8"UTF8, p=ä{50,55}", u8"ä{50,55}" /*pattern*/, true /*partial*/,
                  ZRegex::CodegenOpts() /*encoding*/, "/home/elzarei/bench_data/bad_case.txt")
    ->DenseRange(DFA_LLVM_U8, RE2_TYPE)
    ->Unit(benchmark::kMillisecond);
int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  spdlog::cfg::load_argv_levels(argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}