#pragma once


#include <cstdint>
#include <string>

#include "parser/RegExpVisitor.h"
#include "parser/grammar/RegexLexer.h"
#include "parser/grammar/RegexParser.h"

class RegExp {
public:
  static std::unique_ptr<ZRegex::FiniteAutomaton> GetAutomatonForPattern(
      const std::string& pattern) {
    antlr4::ANTLRInputStream input(pattern);
    RegexLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    RegexParser parser(&tokens);
    auto tree = parser.regex();
    ZRegex::RegExpVisitor regexVisitor;
    auto fa = std::move(tree->accept(&regexVisitor).as<std::unique_ptr<ZRegex::FiniteAutomaton>>());
    fa->Determinize();
//    fa->Visualize();
    return std::move(fa);
  }
};