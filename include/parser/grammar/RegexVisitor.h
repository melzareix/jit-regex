
// Generated from /Users/melzarei/Documents/code-compiling-regex/include/parser/grammar/Regex.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "RegexParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by RegexParser.
 */
class  RegexVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by RegexParser.
   */
    virtual antlrcpp::Any visitRegex(RegexParser::RegexContext *context) = 0;

    virtual antlrcpp::Any visitAlternation(RegexParser::AlternationContext *context) = 0;

    virtual antlrcpp::Any visitExpression(RegexParser::ExpressionContext *context) = 0;

    virtual antlrcpp::Any visitElement(RegexParser::ElementContext *context) = 0;

    virtual antlrcpp::Any visitQuantifier(RegexParser::QuantifierContext *context) = 0;

    virtual antlrcpp::Any visitNumber(RegexParser::NumberContext *context) = 0;

    virtual antlrcpp::Any visitAtom(RegexParser::AtomContext *context) = 0;

    virtual antlrcpp::Any visitCharacter(RegexParser::CharacterContext *context) = 0;

    virtual antlrcpp::Any visitAnyChar(RegexParser::AnyCharContext *context) = 0;

    virtual antlrcpp::Any visitCcPositive(RegexParser::CcPositiveContext *context) = 0;

    virtual antlrcpp::Any visitCcNegative(RegexParser::CcNegativeContext *context) = 0;

    virtual antlrcpp::Any visitClassMember(RegexParser::ClassMemberContext *context) = 0;

    virtual antlrcpp::Any visitRange(RegexParser::RangeContext *context) = 0;

    virtual antlrcpp::Any visitPredefinedClass(RegexParser::PredefinedClassContext *context) = 0;

    virtual antlrcpp::Any visitPredefinedClassName(RegexParser::PredefinedClassNameContext *context) = 0;

    virtual antlrcpp::Any visitRegularCharacter(RegexParser::RegularCharacterContext *context) = 0;

    virtual antlrcpp::Any visitSpecialChar(RegexParser::SpecialCharContext *context) = 0;


};

