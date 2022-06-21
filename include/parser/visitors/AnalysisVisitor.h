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
#include "parser/grammar/RegexVisitor.h"

namespace ZRegex {
  class AnalysisVisitor : public RegexVisitor {
  public:
    bool isComplex = false;
    std::string pattern;

    antlrcpp::Any visitRegex(RegexParser::RegexContext *context) {
      return context->alternation()->accept(this);
    };

    antlrcpp::Any visitAlternation(RegexParser::AlternationContext *context) {
      if (context->expression().size() != 1) {
        isComplex = true;
        return "";
      }
      return context->expression()[0]->accept(this);
    };

    antlrcpp::Any visitExpression(RegexParser::ExpressionContext *context) {
      int idx = 0;
      bool hasWildcard = false;
      for (const auto &expr : context->element()) {
        if (expr == nullptr) continue;
        auto pat = expr->accept(this).as<std::string>();
        if (pat == "%") {
          if (idx != 0 && idx != context->element().size() - 1) {
            isComplex = true;
            return "";
          }
          hasWildcard = true;
        } else {
          pattern += pat;
        }
        if (isComplex) return "";
        idx++;
      }
      isComplex = isComplex || !hasWildcard;
      return pattern;
    }

    antlrcpp::Any visitElement(RegexParser::ElementContext *context) {
      if (context->quantifier()) {
        isComplex = true;
        return "";
      }
      return context->atom()->accept(this).as<std::string>();
    };

    antlrcpp::Any visitQuantifier(RegexParser::QuantifierContext *context) override {
      throw std::runtime_error("ERROR:: This method should not be called!");
    }
    antlrcpp::Any visitNumber(RegexParser::NumberContext *context) override {
      throw std::runtime_error("ERROR:: This method should not be called!");
    }

    antlrcpp::Any visitAtom(RegexParser::AtomContext *context) {
      if (context->characterClass() || context->capture) {
        isComplex = true;
        return "";
      }

      if (context->Wildcard() || context->getText() == "%") {
        return std::string("%");
      }
      return context->character()->accept(this).as<std::string>();
    };

    antlrcpp::Any visitCharacter(RegexParser::CharacterContext *context) {
      if (context->regularCharacter()) {
        return context->regularCharacter()->accept(this).as<std::string>();
      }
      return context->specialChar()->accept(this).as<std::string>();
    };

    antlrcpp::Any visitAnyChar(RegexParser::AnyCharContext *context) { return ""; };

    antlrcpp::Any visitCcNegative(RegexParser::CcNegativeContext *context) { return ""; };

    antlrcpp::Any visitCcPositive(RegexParser::CcPositiveContext *context) { return ""; };

    antlrcpp::Any visitClassMember(RegexParser::ClassMemberContext *context) { return ""; };

    antlrcpp::Any visitRange(RegexParser::RangeContext *context) { return ""; };

    antlrcpp::Any visitPredefinedClass(RegexParser::PredefinedClassContext *context) { return ""; };

    antlrcpp::Any visitPredefinedClassName(RegexParser::PredefinedClassNameContext *context) {
      return "";
    };

    antlrcpp::Any visitRegularCharacter(RegexParser::RegularCharacterContext *context) {
      return context->value->getText();
    };

    antlrcpp::Any visitSpecialChar(RegexParser::SpecialCharContext *context) {
      std::string res = context->value->getText();
      if (context->value->getText() == "n") {
        res = "\n";
      }
      return res;
    };

    antlrcpp::Any visitCcEscapedChar(RegexParser::CcEscapedCharContext *context) { return ""; };

    antlrcpp::Any visitCcUnescapedChar(RegexParser::CcUnescapedCharContext *context) { return ""; };
  };
}  // namespace ZRegex