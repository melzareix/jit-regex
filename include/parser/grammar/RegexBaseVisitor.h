
// Generated from /Users/melzarei/Documents/code-compiling-regex/include/parser/grammar/Regex.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "RegexVisitor.h"


/**
 * This class provides an empty implementation of RegexVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  RegexBaseVisitor : public RegexVisitor {
public:

  virtual antlrcpp::Any visitRegex(RegexParser::RegexContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAlternation(RegexParser::AlternationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpression(RegexParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitElement(RegexParser::ElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQuantifier(RegexParser::QuantifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumber(RegexParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtom(RegexParser::AtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCharacter(RegexParser::CharacterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAnyChar(RegexParser::AnyCharContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCcPositive(RegexParser::CcPositiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCcNegative(RegexParser::CcNegativeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitClassMember(RegexParser::ClassMemberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRange(RegexParser::RangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPredefinedClass(RegexParser::PredefinedClassContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPredefinedClassName(RegexParser::PredefinedClassNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRegularCharacter(RegexParser::RegularCharacterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSpecialChar(RegexParser::SpecialCharContext *ctx) override {
    return visitChildren(ctx);
  }


};

