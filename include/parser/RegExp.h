#pragma once

#include <cstdint>
#include <string>

#include "parser/grammar/RegexLexer.h"
#include "parser/grammar/RegexParser.h"
#include "parser/visitors/RegExpVisitor.h"

class RegExp {
public:
  static bool string_is_valid(const std::string& str) {
    return find_if(str.begin(), str.end(), [](char c) { return !(isalnum(c) || (c == ' ')); })
           == str.end();
  }
  static std::unique_ptr<ZRegex::FiniteAutomaton> GetAutomatonForPattern(const std::string& pattern,
                                                                         bool byte_dfa_utf8
                                                                         = false) {
    auto error_strat = std::shared_ptr<antlr4::BailErrorStrategy>(new antlr4::BailErrorStrategy());
    // std::cout << pattern << std::endl;
    antlr4::ANTLRInputStream input(pattern);
    RegexLexer lexer(&input);
    // lexer.removeErrorListeners();
    antlr4::CommonTokenStream tokens(&lexer);
    RegexParser parser(&tokens);
    parser.setErrorHandler(error_strat);
    auto tree = parser.regex();
    ZRegex::RegExpVisitor regexVisitor(byte_dfa_utf8);
    auto fa = std::move(tree->accept(&regexVisitor).as<std::unique_ptr<ZRegex::FiniteAutomaton>>());
    fa->Determinize(byte_dfa_utf8);
    fa->Visualize();
    return std::move(fa);
  }
};