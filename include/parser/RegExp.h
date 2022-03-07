#pragma once

#include <cstdint>
#include <string>

#include "parser/grammar/RegexLexer.h"
#include "parser/grammar/RegexParser.h"
#include "parser/visitors/RegExpVisitor.h"

class RegExp {
public:
  static std::unique_ptr<ZRegex::FiniteAutomaton> GetAutomatonForPattern(const std::string& pattern,
                                                                         bool byte_dfa_utf8
                                                                         = false) {
    antlr4::ANTLRInputStream input(pattern);
    RegexLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    RegexParser parser(&tokens);
    auto tree = parser.regex();
    ZRegex::RegExpVisitor regexVisitor(byte_dfa_utf8);
    auto fa = std::move(tree->accept(&regexVisitor).as<std::unique_ptr<ZRegex::FiniteAutomaton>>());
    fa->Determinize(byte_dfa_utf8);
    fa->Visualize();
    return std::move(fa);
  }
};