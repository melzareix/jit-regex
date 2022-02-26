
// Generated from /Users/melzarei/Documents/code-compiling-regex/include/parser/grammar/Regex.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  RegexParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, WS = 10, INT = 11, Wildcard = 12, CharacterClassStart = 13, 
    CharacterClassEnd = 14, Caret = 15, Hyphen = 16, Underscore = 17, Pipe = 18, 
    OpenParen = 19, CloseParen = 20, Plus = 21, Asterisk = 22, Qmark = 23, 
    OpenBrace = 24, CloseBrace = 25, Comma = 26, EscapeChar = 27, LETTER = 28
  };

  enum {
    RuleRegex = 0, RuleAlternation = 1, RuleExpression = 2, RuleElement = 3, 
    RuleQuantifier = 4, RuleNumber = 5, RuleAtom = 6, RuleCharacter = 7, 
    RuleCharacterClass = 8, RuleClassMember = 9, RuleRange = 10, RulePredefinedClass = 11, 
    RulePredefinedClassName = 12, RuleRegularCharacter = 13, RuleSpecialChar = 14
  };

  explicit RegexParser(antlr4::TokenStream *input);
  ~RegexParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class RegexContext;
  class AlternationContext;
  class ExpressionContext;
  class ElementContext;
  class QuantifierContext;
  class NumberContext;
  class AtomContext;
  class CharacterContext;
  class CharacterClassContext;
  class ClassMemberContext;
  class RangeContext;
  class PredefinedClassContext;
  class PredefinedClassNameContext;
  class RegularCharacterContext;
  class SpecialCharContext; 

  class  RegexContext : public antlr4::ParserRuleContext {
  public:
    RegexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AlternationContext *alternation();
    antlr4::tree::TerminalNode *EOF();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RegexContext* regex();

  class  AlternationContext : public antlr4::ParserRuleContext {
  public:
    AlternationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Pipe();
    antlr4::tree::TerminalNode* Pipe(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AlternationContext* alternation();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ElementContext *> element();
    ElementContext* element(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionContext* expression();

  class  ElementContext : public antlr4::ParserRuleContext {
  public:
    ElementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AtomContext *atom();
    QuantifierContext *quantifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementContext* element();

  class  QuantifierContext : public antlr4::ParserRuleContext {
  public:
    RegexParser::NumberContext *min = nullptr;
    RegexParser::NumberContext *max = nullptr;
    QuantifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Asterisk();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Qmark();
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<NumberContext *> number();
    NumberContext* number(size_t i);
    antlr4::tree::TerminalNode *Comma();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantifierContext* quantifier();

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> INT();
    antlr4::tree::TerminalNode* INT(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumberContext* number();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    RegexParser::AlternationContext *capture = nullptr;
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CharacterContext *character();
    CharacterClassContext *characterClass();
    antlr4::tree::TerminalNode *Wildcard();
    antlr4::tree::TerminalNode *OpenParen();
    antlr4::tree::TerminalNode *CloseParen();
    AlternationContext *alternation();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtomContext* atom();

  class  CharacterContext : public antlr4::ParserRuleContext {
  public:
    CharacterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RegularCharacterContext *regularCharacter();
    antlr4::tree::TerminalNode *EscapeChar();
    SpecialCharContext *specialChar();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CharacterContext* character();

  class  CharacterClassContext : public antlr4::ParserRuleContext {
  public:
    CharacterClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    CharacterClassContext() = default;
    void copyFrom(CharacterClassContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CcPositiveContext : public CharacterClassContext {
  public:
    CcPositiveContext(CharacterClassContext *ctx);

    RegexParser::ClassMemberContext *positive = nullptr;
    antlr4::tree::TerminalNode *CharacterClassStart();
    antlr4::tree::TerminalNode *CharacterClassEnd();
    std::vector<ClassMemberContext *> classMember();
    ClassMemberContext* classMember(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AnyCharContext : public CharacterClassContext {
  public:
    AnyCharContext(CharacterClassContext *ctx);

    antlr4::tree::TerminalNode *Underscore();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CcNegativeContext : public CharacterClassContext {
  public:
    CcNegativeContext(CharacterClassContext *ctx);

    RegexParser::ClassMemberContext *negative = nullptr;
    antlr4::tree::TerminalNode *CharacterClassStart();
    antlr4::tree::TerminalNode *Caret();
    antlr4::tree::TerminalNode *CharacterClassEnd();
    std::vector<ClassMemberContext *> classMember();
    ClassMemberContext* classMember(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  CharacterClassContext* characterClass();

  class  ClassMemberContext : public antlr4::ParserRuleContext {
  public:
    ClassMemberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CharacterContext *character();
    RangeContext *range();
    PredefinedClassContext *predefinedClass();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassMemberContext* classMember();

  class  RangeContext : public antlr4::ParserRuleContext {
  public:
    RegexParser::CharacterContext *min = nullptr;
    RegexParser::CharacterContext *max = nullptr;
    RangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Hyphen();
    std::vector<CharacterContext *> character();
    CharacterContext* character(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RangeContext* range();

  class  PredefinedClassContext : public antlr4::ParserRuleContext {
  public:
    PredefinedClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PredefinedClassNameContext *predefinedClassName();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredefinedClassContext* predefinedClass();

  class  PredefinedClassNameContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *value = nullptr;
    PredefinedClassNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredefinedClassNameContext* predefinedClassName();

  class  RegularCharacterContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *value = nullptr;
    RegularCharacterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LETTER();
    antlr4::tree::TerminalNode *INT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RegularCharacterContext* regularCharacter();

  class  SpecialCharContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *value = nullptr;
    SpecialCharContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Asterisk();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Qmark();
    antlr4::tree::TerminalNode *CharacterClassStart();
    antlr4::tree::TerminalNode *CharacterClassEnd();
    antlr4::tree::TerminalNode *OpenParen();
    antlr4::tree::TerminalNode *CloseParen();
    antlr4::tree::TerminalNode *Caret();
    antlr4::tree::TerminalNode *Hyphen();
    antlr4::tree::TerminalNode *Underscore();
    antlr4::tree::TerminalNode *Pipe();
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    antlr4::tree::TerminalNode *Wildcard();
    antlr4::tree::TerminalNode *EscapeChar();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SpecialCharContext* specialChar();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

