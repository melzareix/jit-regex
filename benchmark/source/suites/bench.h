/*
 Copyright (c) 2022 Mohamed Elzarei

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <benchmark/benchmark.h>
#include <codegen/codegen.h>
#include <re2/re2.h>
#include <spdlog/spdlog.h>

#include <boost/regex.hpp>
// #include <ctre.hpp>
#include <memory>
#include <regex>
#include <sstream>

// #include "fa/special/epsm.h"
#include "fa/special/epsm_multi.h"
#include "fa/special/kmp.h"
#include "fa/special/simd-avx512f.h"
#include "fa/special/simd.h"
#include "spdlog/cfg/argv.h"
#include "spdlog/pattern_formatter.h"
#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

#define DFA_LLVM_U8 0
#define DFA_LLVM_U32 1
#define DFA_CPP_U8 2
#define DFA_CPP_U32 3
#define RE2_TYPE 4
#define KMP_TYPE_LLVM 5
#define KMP_TYPE_CPP 6
#define KMP_INTERPRETTED 7
#define BOOSTRGX 8
#define STDRGX 9
#define SIMD_SSE42_INTERPRETTED 10

#define INIT                                        \
  std::ifstream st(dataset, std::ios_base::binary); \
  double matches = 0;

#define LABEL                                             \
  if (state.range(0) == DFA_LLVM_U8) {                    \
    state.SetLabel("DFA LLVM U8");                        \
  } else if (state.range(0) == DFA_LLVM_U32) {            \
    state.SetLabel("DFA LLVM U32");                       \
  } else if (state.range(0) == DFA_CPP_U8) {              \
    state.SetLabel("DFA CPP U8");                         \
  } else if (state.range(0) == DFA_CPP_U32) {             \
    state.SetLabel("DFA CPP U32");                        \
  } else if (state.range(0) == RE2_TYPE) {                \
    state.SetLabel("RE2");                                \
  } else if (state.range(0) == KMP_TYPE_LLVM) {           \
    state.SetLabel("KMP LLVM");                           \
  } else if (state.range(0) == KMP_INTERPRETTED) {        \
    state.SetLabel("KMP INTERPRETTED");                   \
  } else if (state.range(0) == KMP_TYPE_CPP) {            \
    state.SetLabel("KMP CPP");                            \
  } else if (state.range(0) == SIMD_SSE42_INTERPRETTED) { \
    state.SetLabel("SIMD INTERPRETTED");                  \
  } else if (state.range(0) == BOOSTRGX) {                \
    state.SetLabel("Boost Regex");                        \
  } else if (state.range(0) == STDRGX) {                  \
    state.SetLabel("Standard Library Regex");             \
  }

#define LOAD_DATA             \
  std::string content;        \
  std::string line;           \
  while (getline(st, line)) { \
    content.append(line);     \
  }                           \
  content.append(append);

#define APPLY_PARTIAL                                                            \
  std::string result;                                                            \
  if (state.range(0) == STDRGX) {                                                \
    result = ".*(" + pattern + ").*";                                            \
  } else if ((state.range(0) >= DFA_LLVM_U8 && state.range(0) <= DFA_CPP_U32)) { \
    result = "%(" + pattern + ")";                                               \
  } else {                                                                       \
    result = pattern;                                                            \
  }

static std::vector<std::string> SplitString(std::string str, std::string delimeter) {
  std::vector<std::string> splittedStrings = {};
  size_t pos = 0;

  while ((pos = str.find(delimeter)) != std::string::npos) {
    std::string token = str.substr(0, pos);
    if (token.length() > 0) splittedStrings.push_back(token);
    str.erase(0, pos + delimeter.length());
  }

  if (str.length() > 0) splittedStrings.push_back(str);
  return splittedStrings;
}

// TODO MORE THAN 2 PATTERNS
static void BENCH_SIMD_EPSM_SMART(benchmark::State& state, const std::string& pattern,
                                  ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  std::string line;
  // assert(pattern.size() <= 32);  // only good if pattern length is small
  state.SetLabel("SIMD EPSMA_SMART Regex");
  auto patterns = SplitString(pattern, "%");
  auto epsm1 = ZRegex::EPSMMatcher(patterns[0].c_str(), patterns[0].size());
  auto epsm2 = ZRegex::EPSMMatcher(patterns[1].c_str(), patterns[1].size());
  for (auto _ : state) {
    while (getline(st, line)) {
      auto p1 = epsm1.epsm2_search_find_first(line.c_str(), line.size());
      auto p2 = epsm2.epsm2_search_find_last(line.c_str(), line.size());
      if (p1 != -1 && p2 != -1 && p2 >= p1) matches += 1;
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_SIMD_EPSM(benchmark::State& state, const std::string& pattern,
                            ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  std::string line;
  // assert(pattern.size() <= 32);  // only good if pattern length is small
  state.SetLabel("SIMD EPSMA Regex");
  auto barr = ZRegex::SIMDSubstringMatch::preprocess(pattern.c_str(), pattern.size());
  auto pcstr = pattern.c_str();
  auto m = pattern.size();
  for (auto _ : state) {
    while (getline(st, line)) {
      // benchmark::DoNotOptimize(
      //     matches += ZRegex::SIMDSubstringMatch::epsm_a(pcstr, m, line.c_str(), line.size(),
      //     barr));
      benchmark::DoNotOptimize(matches
                               += avx2_strstr_anysize_single(line.c_str(), line.size(), pcstr, m));
    }
  }
  state.counters["Matches"] = matches;
}

// TODO MORE THAN 2 PATTERNS
static void BENCH_SIMD_MULTIPATTERN(benchmark::State& state, const std::string& pattern,
                                    ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  std::string line;
  state.SetLabel("SIMD MULTIPATTERN Regex");
  auto patterns = SplitString(pattern, "%");

  for (auto _ : state) {
    auto epsm1 = ZRegex::EPSMMatcher(patterns[0].c_str(), patterns[0].size());
    auto epsm2 = ZRegex::EPSMMatcher(patterns[1].c_str(), patterns[1].size());
    INIT;
    while (getline(st, line)) {
      auto p1 = epsm1.epsma_elz_early(line.c_str(), line.size());
      auto p2 = epsm2.epsma_elz(line.c_str(), line.size());
      if (p1 != -1 && p2 != -1 && p2 >= p1) matches += 1;
    }
    state.counters["Matches"] = matches;
  }
}

static void BENCH_BOOST(benchmark::State& state, const std::string& pattern,
                        ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  std::string line;
  state.SetLabel("Boost Regex");
  auto result = ".*(" + pattern + ").*";
  for (auto _ : state) {
    INIT;
    auto regex = boost::regex(result);
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += boost::regex_match(line.c_str(), regex) ? 1 : 0);
    }
    state.counters["Matches"] = matches;
  }
}

static void BENCH_STDLIB(benchmark::State& state, const std::string& pattern,
                         ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  std::string line;
  state.SetLabel("std::regex");
  auto result = ".*(" + pattern + ").*";
  std::regex* regex = nullptr;
  for (auto _ : state) {
    if (regex == nullptr) regex = new std::regex(result);
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += std::regex_match(line, *regex) ? 1 : 0);
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_RE2(benchmark::State& state, const std::string& pattern,
                      ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  std::string line;
  state.SetLabel("RE2");
  auto re2_enc = (opts.IsUTF8() || opts.IsUTF32()) ? re2::RE2::DefaultOptions : re2::RE2::Latin1;
  for (auto _ : state) {
    auto p = re2::RE2(pattern, re2_enc);
    INIT;
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += re2::RE2::PartialMatch(line, p));
    }
    state.counters["Matches"] = matches;
  }
}

static void BENCH_SIMD_INTERPRETTED(benchmark::State& state, const std::string& pattern,
                                    ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  state.SetLabel("SIMD INTERPRETTED");
  std::string line;
  for (auto _ : state) {
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += (ZRegex::SIMDSubstringMatch::sse4_strstr_anysize(
                                   line.c_str(), line.size(), pattern.c_str(), pattern.size())));
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_KMP_INTERPRETTED(benchmark::State& state, const std::string& pattern,
                                   ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  state.SetLabel("KMP INTERPRETTED");
  std::string line;
  ZRegex::KMPAlgorithm kmp(pattern);
  kmp.BuildTable();
  for (auto _ : state) {
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += kmp.Search(line));
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_KMP_DFA(benchmark::State& state, const std::string& pattern,
                          ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT LABEL APPLY_PARTIAL;
  std::string line;
  if (state.range(0) == KMP_TYPE_LLVM) {
    opts.SetByteDFA(true);
    opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
    opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::LLVM);
  } else if (state.range(0) == KMP_TYPE_CPP) {
    opts.SetByteDFA(true);
    opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::UTF8);
    opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::CPP);
  }
  ZRegex::Codegen code_generator(opts);
  for (auto _ : state) {
    code_generator.CompileKMP(pattern.c_str());
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += code_generator.Run(line.c_str()));
    }
  }
  state.counters["Matches"] = matches;
}

static void BENCH_PCRE2_DFA(benchmark::State& state, const std::string& pattern,
                            ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  std::string line;
  state.SetLabel("PCRE2_DFA");
  pcre2_code* re = nullptr;
  pcre2_match_data* match_data = nullptr;

  int errorcode;
  PCRE2_SIZE erroroffset;
  PCRE2_SPTR p = (PCRE2_SPTR)(pattern.c_str());
  // match_data = pcre2_match_data_create(re, nullptr);
  int dfa_workspace[100];

  for (auto _ : state) {
    while (getline(st, line)) {
      if (re == nullptr) {
        match_data = pcre2_match_data_create(0, nullptr);
        re = pcre2_compile(p, pattern.size(), PCRE2_UCP | PCRE2_UTF, &errorcode, &erroroffset,
                           nullptr);
      }

      auto rt = pcre2_dfa_match(re, (PCRE2_SPTR)(line.c_str()), line.size(), 0,
                                PCRE2_NO_UTF_CHECK | PCRE2_DFA_SHORTEST, match_data, nullptr,
                                dfa_workspace, 100);

      // spdlog::error(rt);
      if (rt >= 0) matches += 1;
    }
    state.counters["Matches"] = matches;
  }
}

static void BENCH_PCRE2(benchmark::State& state, const std::string& pattern,
                        ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  std::string line;
  state.SetLabel("PCRE2");
  for (auto _ : state) {
    INIT;
    pcre2_code* re = nullptr;
    pcre2_match_data* match_data = nullptr;

    int errorcode;
    PCRE2_SIZE erroroffset;
    PCRE2_SPTR p = (PCRE2_SPTR)(pattern.c_str());
    re = pcre2_compile(p, pattern.size(), PCRE2_UCP | PCRE2_UTF, &errorcode, &erroroffset, nullptr);
    match_data = pcre2_match_data_create_from_pattern(re, nullptr);
    auto err = pcre2_jit_compile(re, PCRE2_JIT_COMPLETE);

    while (getline(st, line)) {
      int rt = -1;
      benchmark::DoNotOptimize(rt = pcre2_jit_match(re, (PCRE2_SPTR)(line.c_str()), line.size(), 0,
                                                    PCRE2_NO_UTF_CHECK, match_data, nullptr));

      if (rt >= 0) matches += 1;
      state.counters["Matches"] = matches;
    }
  }
}
static void BENCH_DFA(benchmark::State& state, const std::string& pattern,
                      ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  LABEL APPLY_PARTIAL;
  std::string line;
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
  for (auto _ : state) {
    INIT;
    ZRegex::Codegen code_generator(opts);
    code_generator.Compile(result.c_str());
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += code_generator.Run(line.c_str()));
    }
    state.counters["Matches"] = matches;
  }
}