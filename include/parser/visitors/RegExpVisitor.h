//
// Created by Mohamed El Zarei on 08/01/2022.
//

#ifndef ZREGEXSTANDALONE_REGEXPVISITOR_H
#define ZREGEXSTANDALONE_REGEXPVISITOR_H

#include <memory>
#include <vector>

#include "fa/fa.h"
#include "fa/factory.h"
#include "parser/grammar/RegexVisitor.h"
#include "spdlog/fmt/fmt.h"

namespace ZRegex {
  class RegExpVisitor : public RegexVisitor {
  private:
    bool byte_dfa_utf8_;

  public:
    RegExpVisitor(bool byte_dfa_utf8 = false) : byte_dfa_utf8_(byte_dfa_utf8){};
    antlrcpp::Any visitAlternation(RegexParser::AlternationContext *context) override {
      // single element return it
      if (context->expression().size() == 1) {
        auto r = std::move(
            context->expression(0)->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        return r;
      }
      std::vector<std::unique_ptr<FiniteAutomaton>> automatons;
      for (const auto &expr : context->expression()) {
        auto expr_nfa = std::move(expr->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        automatons.push_back(std::move(expr_nfa));
      }

      return FAFactory::UnionAll(automatons);
    }

  public:
    antlrcpp::Any visitRegex(RegexParser::RegexContext *context) override {
      return context->alternation()->accept(this);
    }
    antlrcpp::Any visitExpression(RegexParser::ExpressionContext *context) override {
      // single element
      if (context->element().size() == 1) {
        auto z
            = std::move(context->element(0)->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        return z;
      }

      // concat all elements
      std::vector<std::unique_ptr<FiniteAutomaton>> automatons;
      for (const auto &expr : context->element()) {
        if (expr == nullptr) continue;
        auto z = std::move(expr->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        automatons.push_back(std::move(z));
      }
      return ZRegex::FAFactory::ConcatAll(automatons);
    }

    antlrcpp::Any visitElement(RegexParser::ElementContext *context) override {
      auto atm = std::move(context->atom()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      if (!context->quantifier()) {
        return atm;
      }
      auto min = context->quantifier()->min;
      auto max = context->quantifier()->max;
      if (context->quantifier()->Plus()) {
        return ZRegex::FAFactory::RepeatMinimum(std::move(atm), 1);
      } else if (context->quantifier()->Asterisk()) {
        return ZRegex::FAFactory::KleeneStar(std::move(atm));
      } else if (context->quantifier()->Qmark()) {
        return ZRegex::FAFactory::Optional(std::move(atm));
      } else if (!context->quantifier()->Comma()) {  // no comma thus exact match
        auto min_reps = std::stoi(min->getText());
        return ZRegex::FAFactory::RepeatMinMax(std::move(atm), min_reps, min_reps);
      } else if (min && max) {  // min and max defined
        auto min_reps = std::stoi(min->getText());
        auto max_reps = std::stoi(max->getText());
        if (max_reps < min_reps) {
          throw std::runtime_error(
              fmt::format("Max {} must be less than or equal to Min {}.", max_reps, min_reps));
        }
        return ZRegex::FAFactory::RepeatMinMax(std::move(atm), min_reps, max_reps);
      } else if (min) {  // min only defined and comma is defined thus at least min
        auto min_reps = min->getText();
        return ZRegex::FAFactory::RepeatMinimum(std::move(atm), std::stoi(min_reps));
      }
      return atm;
    }

    antlrcpp::Any visitQuantifier(RegexParser::QuantifierContext *context) override {
      throw std::runtime_error("ERROR:: This method should not be called!");
    }
    antlrcpp::Any visitNumber(RegexParser::NumberContext *context) override {
      throw std::runtime_error("ERROR:: This method should not be called!");
    }
    antlrcpp::Any visitAtom(RegexParser::AtomContext *context) override {
      if (context->Wildcard()) {
        auto c = ZRegex::FAFactory::AnyChar(byte_dfa_utf8_);
        return ZRegex::FAFactory::KleeneStar(std::move(c));
      }
      if (context->capture) {
        auto v = std::move(context->capture->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        return std::move(v);
      }

      if (context->characterClass()) {
        auto v = std::move(
            context->characterClass()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        return std::move(v);
      }

      auto v = std::move(context->character()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      return std::move(v);
    }

    antlrcpp::Any visitCharacter(RegexParser::CharacterContext *context) override {
      std::unique_ptr<FiniteAutomaton> v;
      if (context->specialChar()) {
        v = std::move(context->specialChar()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      } else {
        v = std::move(
            context->regularCharacter()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      }
      return std::move(v);
    };

    antlrcpp::Any visitRegularCharacter(RegexParser::RegularCharacterContext *context) override {
      if (byte_dfa_utf8_) {
        return ZRegex::FAFactory::ByteAutomaton(context->value->getText());
      }
      return ZRegex::FAFactory::StringAutomaton(context->value->getText());
    }

    antlrcpp::Any visitSpecialChar(RegexParser::SpecialCharContext *context) override {
      if (byte_dfa_utf8_) {
        if (context->value->getText() == "n") {
          return ZRegex::FAFactory::ByteAutomaton("\n");
        }
        return ZRegex::FAFactory::ByteAutomaton(context->value->getText());
      }
      return ZRegex::FAFactory::StringAutomaton(context->value->getText());
    }

    antlrcpp::Any visitAnyChar(RegexParser::AnyCharContext *context) override {
      return ZRegex::FAFactory::AnyChar(byte_dfa_utf8_);
    }
    antlrcpp::Any visitCcPositive(RegexParser::CcPositiveContext *context) override {
      std::vector<std::unique_ptr<FiniteAutomaton>> automatons;
      for (const auto &expr : context->classMember()) {
        auto expr_nfa = std::move(expr->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        automatons.push_back(std::move(expr_nfa));
      }

      return FAFactory::UnionAll(automatons);
    }
    antlrcpp::Any visitCcNegative(RegexParser::CcNegativeContext *context) override {
      // negation is equivalent to (.&~(cc))
      std::vector<std::unique_ptr<FiniteAutomaton>> automatons;
      for (const auto &expr : context->classMember()) {
        auto expr_nfa = std::move(expr->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        automatons.push_back(std::move(expr_nfa));
      }

      auto fa1 = FAFactory::UnionAll(automatons);
      fa1->Complement(byte_dfa_utf8_);
      fa1->Visualize();

      auto fa2 = ZRegex::FAFactory::AnyChar(byte_dfa_utf8_);
      fa2->Visualize();
      return FAFactory::Intersect(std::move(fa2), std::move(fa1));
    }

    antlrcpp::Any visitCcEscapedChar(RegexParser::CcEscapedCharContext *context) {
      if (byte_dfa_utf8_) {
        if (context->value->getText() == "n") {
          return ZRegex::FAFactory::ByteAutomaton("\n");
        }
        return ZRegex::FAFactory::ByteAutomaton(context->value->getText());
      }
      return ZRegex::FAFactory::StringAutomaton(context->value->getText());
    }

    antlrcpp::Any visitCcUnescapedChar(RegexParser::CcUnescapedCharContext *context) {
      if (byte_dfa_utf8_) {
        return ZRegex::FAFactory::ByteAutomaton(context->value->getText());
      }
      return ZRegex::FAFactory::StringAutomaton(context->value->getText());
    }

    antlrcpp::Any visitClassMember(RegexParser::ClassMemberContext *context) override {
      if (context->range()) {
        return std::move(context->range()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      }

      if (context->predefinedClass()) {
        return std::move(
            context->predefinedClass()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      }
      // default case it is a character
      if (context->regularCharacter()) {
        return std::move(
            context->regularCharacter()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      }

      if (context->ccEscapedChar()) {
        return std::move(
            context->ccEscapedChar()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
      }

      return std::move(
          context->ccUnescapedChar()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
    }
    antlrcpp::Any visitRange(RegexParser::RangeContext *context) override {
      if (!byte_dfa_utf8_) {
        auto min = std::move(context->min->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        auto max = std::move(context->max->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
        auto minx = min.get();
        auto maxx = max.get();
        auto min_char = *min->initial_state->transitions.begin();
        auto max_char = *max->initial_state->transitions.begin();

        return ZRegex::FAFactory::CharRangeAutomaton(min_char.min, max_char.min);
      }
      std::string rs = "", re = "";
      if (context->min->regularCharacter()) {
        rs = context->min->regularCharacter()->getText();
      } else if (context->min->specialChar()) {
        rs = context->min->specialChar()->getText();
      }

      if (context->max->regularCharacter()) {
        re = context->max->regularCharacter()->getText();
      } else if (context->max->specialChar()) {
        re = context->max->specialChar()->getText();
      }
      auto range_ = UTF8Range(rs, re);
      std::vector<std::vector<std::vector<uint8_t>>> ranges;
      while (range_.hasNext()) {
        ranges.push_back(range_.NextRange());
      }
      return FAFactory::RangeDFA(ranges);
    }
    antlrcpp::Any visitPredefinedClass(RegexParser::PredefinedClassContext *context) override {
      return std::move(
          context->predefinedClassName()->accept(this).as<std::unique_ptr<FiniteAutomaton>>());
    }
    antlrcpp::Any visitPredefinedClassName(
        RegexParser::PredefinedClassNameContext *context) override {
      auto token = context->value->getText();
      if (token == "ALPHA") {
        return FAFactory::AlphaCharClass();
      } else if (token == "UPPER") {
        return FAFactory::UpperCharClass();
      } else if (token == "LOWER") {
        return FAFactory::LowerCharClass();
      } else if (token == "DIGIT") {
        return FAFactory::DigitCharClass();
      } else if (token == "ALNUM") {
        return FAFactory::AlphaNumCharClass();
      } else if (token == "SPACE") {
        throw std::runtime_error("Space not implemented!");
      } else if (token == "WHITESPACE") {
        throw std::runtime_error("WhiteSpace not implemented!");
      }
      throw std::runtime_error("INVALID Predefined Class!");
    }
  };
}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_REGEXPVISITOR_H
