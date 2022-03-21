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
#define KMP_TYPE_LLVM 5
#define KMP_TYPE_CPP 6
#define KMP_INTERPRETTED 7
#define BOOSTRGX 8
#define STDRGX 9

#define INIT                                        \
  std::ifstream st(dataset, std::ios_base::binary); \
  double matches = 0;

#define LABEL                                      \
  if (state.range(0) == DFA_LLVM_U8) {             \
    state.SetLabel("DFA LLVM U8");                 \
  } else if (state.range(0) == DFA_LLVM_U32) {     \
    state.SetLabel("DFA LLVM U32");                \
  } else if (state.range(0) == DFA_CPP_U8) {       \
    state.SetLabel("DFA CPP U8");                  \
  } else if (state.range(0) == DFA_CPP_U32) {      \
    state.SetLabel("DFA CPP U32");                 \
  } else if (state.range(0) == RE2_TYPE) {         \
    state.SetLabel("RE2");                         \
  } else if (state.range(0) == KMP_TYPE_LLVM) {    \
    state.SetLabel("KMP LLVM");                    \
  } else if (state.range(0) == KMP_INTERPRETTED) { \
    state.SetLabel("KMP INTERPRETTED");            \
  } else if (state.range(0) == KMP_TYPE_CPP) {     \
    state.SetLabel("KMP CPP");                     \
  } else if (state.range(0) == BOOSTRGX) {         \
    state.SetLabel("Boost Regex");                 \
  } else if (state.range(0) == STDRGX) {           \
    state.SetLabel("Standard Library Regex");      \
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

static void BENCH_BOOST(benchmark::State& state, const std::string& pattern,
                        ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT;
  std::string line;
  state.SetLabel("Boost Regex");
  auto result = ".*(" + pattern + ").*";
  boost::regex* regex = nullptr;
  for (auto _ : state) {
    if (regex == nullptr) regex = new boost::regex(result);
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += boost::regex_match(line, *regex) ? 1 : 0);
    }
  }
  state.counters["Matches"] = matches;
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
  INIT;
  std::string line;
  state.SetLabel("RE2");
  auto re2_enc = (opts.IsUTF8() || opts.IsUTF32()) ? re2::RE2::DefaultOptions : re2::RE2::Latin1;
  re2::RE2* p = nullptr;
  for (auto _ : state) {
    if (p == nullptr) p = new re2::RE2(pattern, re2_enc);
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += re2::RE2::PartialMatch(line, *p));
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

static void BENCH_DFA(benchmark::State& state, const std::string& pattern,
                      ZRegex::CodegenOpts&& opts, const std::string& dataset) {
  INIT LABEL APPLY_PARTIAL;
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
  ZRegex::Codegen code_generator(opts);
  for (auto _ : state) {
    code_generator.Compile(result.c_str());
    while (getline(st, line)) {
      benchmark::DoNotOptimize(matches += code_generator.Run(line.c_str()));
    }
  }
  state.counters["Matches"] = matches;
}