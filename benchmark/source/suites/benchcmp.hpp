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

#include <memory>
#include <regex>
#include <sstream>

// #include "fa/special/epsm.h"
#include "fa/special/kmp.h"
#include "fa/special/simd.h"
// #include "fa/special/smart-epsm.h"
#include "fa/special/simd-avx512f.h"
#include "spdlog/cfg/argv.h"
#include "spdlog/pattern_formatter.h"

#define DFA_LLVM_U8 0
#define DFA_CPP_U8 1
#define DFA_LLVM_U32 2
#define DFA_CPP_U32 3
#define DFA_LLVM_ASCII 4
#define DFA_CPP_ASCII 5

#define INIT                                        \
  std::ifstream st(dataset, std::ios_base::binary); \
  double errors = 0;

#define LABEL                                    \
  if (state.range(0) == DFA_LLVM_U8) {           \
    state.SetLabel("DFA LLVM U8");               \
  } else if (state.range(0) == DFA_LLVM_U32) {   \
    state.SetLabel("DFA LLVM U32");              \
  } else if (state.range(0) == DFA_CPP_U8) {     \
    state.SetLabel("DFA CPP U8");                \
  } else if (state.range(0) == DFA_CPP_U32) {    \
    state.SetLabel("DFA CPP U32");               \
  } else if (state.range(0) == DFA_LLVM_ASCII) { \
    state.SetLabel("DFA LLVM ASCII");            \
  } else if (state.range(0) == DFA_CPP_ASCII) {  \
    state.SetLabel("DFA CPP ASCII");             \
  } else {                                       \
    state.SetLabel("RE2");                       \
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

static void BENCH_DFA(benchmark::State& state, ZRegex::CodegenOpts&& opts,
                      const std::string& dataset) {
  LABEL;
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
  } else if (state.range(0) == DFA_LLVM_ASCII) {
    opts.SetByteDFA(false);
    opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::ASCII);
    opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::LLVM);
  } else if (state.range(0) == DFA_CPP_ASCII) {
    opts.SetByteDFA(false);
    opts.SetEncoding(ZRegex::CodegenOpts::DFAEncoding::ASCII);
    opts.SetBackendType(ZRegex::CodegenOpts::CodegenBackendType::CPP);
  }
  ZRegex::Codegen code_generator(opts);
  for (auto _ : state) {
    INIT;
    while (getline(st, line)) {
      try {
        code_generator.CompileForBenchmark(line.c_str());
      } catch (const std::exception& e) {
        spdlog::error(e.what());
        errors += 1;
        continue;
      }
    }
    state.counters["Errors"] = errors;
  }
}