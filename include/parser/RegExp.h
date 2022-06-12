#pragma once

#include <cstdint>
#include <string>

#include "parser/grammar/RegexLexer.h"
#include "parser/grammar/RegexParser.h"
#include "parser/visitors/RegExpVisitor.h"

namespace ZRegex {
  class RegExp {
  public:
    static std::unique_ptr<ZRegex::FiniteAutomaton> GetAutomatonForPattern(
        const std::string& pattern, bool byte_dfa_utf8 = false) {
      auto error_strat
          = std::shared_ptr<antlr4::BailErrorStrategy>(new antlr4::BailErrorStrategy());
      antlr4::ANTLRInputStream input(pattern);
      RegexLexer lexer(&input);
      antlr4::CommonTokenStream tokens(&lexer);
      RegexParser parser(&tokens);
      parser.setErrorHandler(error_strat);
      auto tree = parser.regex();
      ZRegex::RegExpVisitor regexVisitor(byte_dfa_utf8);

      auto fa
          = std::move(tree->accept(&regexVisitor).as<std::unique_ptr<ZRegex::FiniteAutomaton>>());
      // fa->Visualize();
      fa->Determinize(byte_dfa_utf8);
      return std::move(fa);
    }
  };
};  // namespace ZRegex
