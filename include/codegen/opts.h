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

namespace ZRegex {
  class CodegenOpts {
  public:
    enum CodegenBackendType { LLVM, CPP };
    enum DFAEncoding { UTF8, ASCII };
    enum OptimizationLevel { O1, O2, O3 };
    enum AutomatonType { DFA, PARABIX };
    CodegenOpts(const CodegenBackendType& backendtype = LLVM, const DFAEncoding& encoding = UTF8,
                const OptimizationLevel& olevel = O2, const bool& utf8_byte_dfa = false)
        : backendtype_(backendtype),
          encoding_(encoding),
          optimization_level_(olevel),
          utf8_byte_dfa_(utf8_byte_dfa){};

    CodegenBackendType GetBackendType() const { return backendtype_; };
    DFAEncoding GetEncoding() const { return encoding_; };

  private:
    CodegenBackendType backendtype_;
    DFAEncoding encoding_;
    OptimizationLevel optimization_level_;
    bool utf8_byte_dfa_;  // a byte dfa where utf8 is encoded into the dfa itself
  };
};  // namespace ZRegex
