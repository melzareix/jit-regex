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
#include <llvm/Passes/PassBuilder.h>

namespace ZRegex {
  class CodegenOpts {
  public:
    enum CodegenBackendType { LLVM, CPP };
    enum DFAEncoding { UTF8, ASCII };
    enum OptimizationLevel { O0, O1, O2, O3, Os, Oz };
    CodegenOpts(const CodegenBackendType& backendtype = LLVM, const DFAEncoding& encoding = UTF8,
                const OptimizationLevel& olevel = O2, const bool& utf8_byte_dfa = false)
        : backendtype_(backendtype),
          encoding_(encoding),
          optimization_level_(olevel),
          utf8_byte_dfa_(utf8_byte_dfa){};

    CodegenBackendType GetBackendType() const { return backendtype_; };
    DFAEncoding GetEncoding() const { return encoding_; };
    bool IsByteDFA() const { return utf8_byte_dfa_; }
    bool IsUTF32() const { return encoding_ == UTF8 && !IsByteDFA(); }
    bool IsUTF8() const { return IsByteDFA() && encoding_ == UTF8; }
    bool IsAscii() const { return encoding_ == ASCII; }

    void SetEncoding(const DFAEncoding& encoding) { encoding_ = encoding; }
    void SetBackendType(const CodegenBackendType& backendType) { backendtype_ = backendType; }
    void SetByteDFA(const bool& byteDfa) { utf8_byte_dfa_ = byteDfa; }

    std::string getOptimizationLevelString() const {
      if (optimization_level_ == O0) return "O0";
      if (optimization_level_ == O1) return "O1";
      if (optimization_level_ == O2) return "O2";
      if (optimization_level_ == O3) return "O3";
      if (optimization_level_ == Oz) return "Oz";
      if (optimization_level_ == Os) return "Os";
      return "";
    }

    llvm::PassBuilder::OptimizationLevel getOptimizationLevelforLLVM() const {
      if (optimization_level_ == O0) return llvm::PassBuilder::OptimizationLevel::O0;
      if (optimization_level_ == O1) return llvm::PassBuilder::OptimizationLevel::O1;
      if (optimization_level_ == O2) return llvm::PassBuilder::OptimizationLevel::O2;
      if (optimization_level_ == O3) return llvm::PassBuilder::OptimizationLevel::O3;
      if (optimization_level_ == Oz) return llvm::PassBuilder::OptimizationLevel::Oz;
      if (optimization_level_ == Os) return llvm::PassBuilder::OptimizationLevel::Os;
      return llvm::PassBuilder::OptimizationLevel::O2;
    }

  private:
    CodegenBackendType backendtype_;
    DFAEncoding encoding_;
    OptimizationLevel optimization_level_;
    bool utf8_byte_dfa_;  // a byte dfa where utf8 is encoded into the dfa itself
  };
};  // namespace ZRegex
