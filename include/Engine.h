#pragma once

#include <cstdint>
#include <string>

#include "codegen/codegen.h"
#include "fa/special/epsm_multi.h"
#include "parser/grammar/RegexLexer.h"
#include "parser/grammar/RegexParser.h"
#include "parser/visitors/AnalysisVisitor.h"
#include "parser/visitors/RegExpVisitor.h"

namespace ZRegex {
  class Engine {
  private:
    const std::string pattern;
    char* simdPattern;
    const CodegenOpts opts;
    EPSMMatcher* matcher;

    // parser variable
    antlr4::ANTLRInputStream input;
    RegexLexer lexer;
    antlr4::CommonTokenStream tokens;
    RegexParser parser;

    ZRegex::Codegen codegen;
    bool isSimd = false;

  public:
    Engine(const std::string& pattern, const CodegenOpts& opts)
        : opts(opts),
          pattern(pattern),
          codegen(opts),
          input(pattern),
          lexer(&input),
          tokens(&lexer),
          parser(&tokens) {
      Parse();
      Compile();
    }

    ~Engine() {
      if (matcher != nullptr) {
        delete matcher;
      }
    }
    void CompileSIMDMatcher(std::string& pat) {
      if (pattern.size() <= 16) {
        matcher = new EPSMMatcher(pat);
      }
      // TODO:: general simd matcher
    }

    void Parse() {
      auto error_strat
          = std::shared_ptr<antlr4::BailErrorStrategy>(new antlr4::BailErrorStrategy());
      parser.setErrorHandler(error_strat);
    }

    void Compile() {
      ZRegex::AnalysisVisitor analysisVisitor;
      auto tree = parser.regex();
      tree->accept(&analysisVisitor);
      if (!analysisVisitor.isComplex && analysisVisitor.pattern.size() <= 16) {
        isSimd = true;
        // simdPattern = const_cast<char*>(analysisVisitor.pattern.c_str());
        CompileSIMDMatcher(analysisVisitor.pattern);
        return;
      }

      // simd not suitable, compile dfa
      bool byteDfa = opts.IsByteDFA();
      ZRegex::RegExpVisitor regexVisitor(byteDfa);
      auto fa
          = std::move(tree->accept(&regexVisitor).as<std::unique_ptr<ZRegex::FiniteAutomaton>>());
      fa->Determinize(byteDfa);
      this->codegen.GenerateCodeAndCompile(std::move(fa), pattern);
    }
    bool Run(std::string text) {
      if (isSimd) {
        spdlog::debug("Running via SIMD.");
        auto ret = matcher->epsma_elz_early(text.c_str(), text.size());
        return ret != -1;
      }
      spdlog::debug("Running via DFA.");
      return codegen.Run(text.c_str());
    }
  };
};  // namespace ZRegex
