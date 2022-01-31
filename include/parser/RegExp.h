#pragma once

#include <parser/RegExpHelpers.h>
#include <parser/nodes/IBaseNode.h>
#include <stdint.h>

#include <string>
#include "parser/AntlrNodeVisitor.h"
#include "parser/grammar/regexLexer.h"
#include "parser/grammar/regexParser.h"
class RegExp {
public:
  static std::unique_ptr<ZRegex::FiniteAutomaton> GetAutomatonForPattern(const std::string& pattern) {
    antlr4::ANTLRInputStream input(pattern);
    regexLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    regexParser parser(&tokens);
    auto tree = parser.regex();
    ZRegex::AntlrNodeVisitor regexVisitor;
    auto r = std::move(tree->accept(&regexVisitor).as<std::unique_ptr<ZRegex::FiniteAutomaton>>());
    r->Determinize();
    r->Visualize();
    return std::move(r);
  }
};