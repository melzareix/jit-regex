
// Generated from /Users/melzarei/Documents/code-compiling-regex/include/parser/grammar/Regex.g4 by ANTLR 4.9.3


#include "RegexVisitor.h"

#include "RegexParser.h"


using namespace antlrcpp;
using namespace antlr4;

RegexParser::RegexParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

RegexParser::~RegexParser() {
  delete _interpreter;
}

std::string RegexParser::getGrammarFileName() const {
  return "Regex.g4";
}

const std::vector<std::string>& RegexParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& RegexParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- RegexContext ------------------------------------------------------------------

RegexParser::RegexContext::RegexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RegexParser::AlternationContext* RegexParser::RegexContext::alternation() {
  return getRuleContext<RegexParser::AlternationContext>(0);
}

tree::TerminalNode* RegexParser::RegexContext::EOF() {
  return getToken(RegexParser::EOF, 0);
}


size_t RegexParser::RegexContext::getRuleIndex() const {
  return RegexParser::RuleRegex;
}


antlrcpp::Any RegexParser::RegexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitRegex(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::RegexContext* RegexParser::regex() {
  RegexContext *_localctx = _tracker.createInstance<RegexContext>(_ctx, getState());
  enterRule(_localctx, 0, RegexParser::RuleRegex);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(30);
    alternation();
    setState(31);
    match(RegexParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AlternationContext ------------------------------------------------------------------

RegexParser::AlternationContext::AlternationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RegexParser::ExpressionContext *> RegexParser::AlternationContext::expression() {
  return getRuleContexts<RegexParser::ExpressionContext>();
}

RegexParser::ExpressionContext* RegexParser::AlternationContext::expression(size_t i) {
  return getRuleContext<RegexParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> RegexParser::AlternationContext::Pipe() {
  return getTokens(RegexParser::Pipe);
}

tree::TerminalNode* RegexParser::AlternationContext::Pipe(size_t i) {
  return getToken(RegexParser::Pipe, i);
}


size_t RegexParser::AlternationContext::getRuleIndex() const {
  return RegexParser::RuleAlternation;
}


antlrcpp::Any RegexParser::AlternationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitAlternation(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::AlternationContext* RegexParser::alternation() {
  AlternationContext *_localctx = _tracker.createInstance<AlternationContext>(_ctx, getState());
  enterRule(_localctx, 2, RegexParser::RuleAlternation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(33);
    expression();
    setState(38);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RegexParser::Pipe) {
      setState(34);
      match(RegexParser::Pipe);
      setState(35);
      expression();
      setState(40);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

RegexParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RegexParser::ElementContext *> RegexParser::ExpressionContext::element() {
  return getRuleContexts<RegexParser::ElementContext>();
}

RegexParser::ElementContext* RegexParser::ExpressionContext::element(size_t i) {
  return getRuleContext<RegexParser::ElementContext>(i);
}


size_t RegexParser::ExpressionContext::getRuleIndex() const {
  return RegexParser::RuleExpression;
}


antlrcpp::Any RegexParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::ExpressionContext* RegexParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 4, RegexParser::RuleExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(44);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RegexParser::INT)
      | (1ULL << RegexParser::Wildcard)
      | (1ULL << RegexParser::CharacterClassStart)
      | (1ULL << RegexParser::Underscore)
      | (1ULL << RegexParser::OpenParen)
      | (1ULL << RegexParser::EscapeChar)
      | (1ULL << RegexParser::LETTER))) != 0)) {
      setState(41);
      element();
      setState(46);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementContext ------------------------------------------------------------------

RegexParser::ElementContext::ElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RegexParser::AtomContext* RegexParser::ElementContext::atom() {
  return getRuleContext<RegexParser::AtomContext>(0);
}

RegexParser::QuantifierContext* RegexParser::ElementContext::quantifier() {
  return getRuleContext<RegexParser::QuantifierContext>(0);
}


size_t RegexParser::ElementContext::getRuleIndex() const {
  return RegexParser::RuleElement;
}


antlrcpp::Any RegexParser::ElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitElement(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::ElementContext* RegexParser::element() {
  ElementContext *_localctx = _tracker.createInstance<ElementContext>(_ctx, getState());
  enterRule(_localctx, 6, RegexParser::RuleElement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(47);
    atom();
    setState(49);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RegexParser::Plus)
      | (1ULL << RegexParser::Asterisk)
      | (1ULL << RegexParser::Qmark)
      | (1ULL << RegexParser::OpenBrace))) != 0)) {
      setState(48);
      quantifier();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuantifierContext ------------------------------------------------------------------

RegexParser::QuantifierContext::QuantifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RegexParser::QuantifierContext::Asterisk() {
  return getToken(RegexParser::Asterisk, 0);
}

tree::TerminalNode* RegexParser::QuantifierContext::Plus() {
  return getToken(RegexParser::Plus, 0);
}

tree::TerminalNode* RegexParser::QuantifierContext::Qmark() {
  return getToken(RegexParser::Qmark, 0);
}

tree::TerminalNode* RegexParser::QuantifierContext::OpenBrace() {
  return getToken(RegexParser::OpenBrace, 0);
}

tree::TerminalNode* RegexParser::QuantifierContext::CloseBrace() {
  return getToken(RegexParser::CloseBrace, 0);
}

std::vector<RegexParser::NumberContext *> RegexParser::QuantifierContext::number() {
  return getRuleContexts<RegexParser::NumberContext>();
}

RegexParser::NumberContext* RegexParser::QuantifierContext::number(size_t i) {
  return getRuleContext<RegexParser::NumberContext>(i);
}

tree::TerminalNode* RegexParser::QuantifierContext::Comma() {
  return getToken(RegexParser::Comma, 0);
}


size_t RegexParser::QuantifierContext::getRuleIndex() const {
  return RegexParser::RuleQuantifier;
}


antlrcpp::Any RegexParser::QuantifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitQuantifier(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::QuantifierContext* RegexParser::quantifier() {
  QuantifierContext *_localctx = _tracker.createInstance<QuantifierContext>(_ctx, getState());
  enterRule(_localctx, 8, RegexParser::RuleQuantifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(64);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RegexParser::Asterisk: {
        enterOuterAlt(_localctx, 1);
        setState(51);
        match(RegexParser::Asterisk);
        break;
      }

      case RegexParser::Plus: {
        enterOuterAlt(_localctx, 2);
        setState(52);
        match(RegexParser::Plus);
        break;
      }

      case RegexParser::Qmark: {
        enterOuterAlt(_localctx, 3);
        setState(53);
        match(RegexParser::Qmark);
        break;
      }

      case RegexParser::OpenBrace: {
        enterOuterAlt(_localctx, 4);
        setState(54);
        match(RegexParser::OpenBrace);
        setState(55);
        antlrcpp::downCast<QuantifierContext *>(_localctx)->min = number();
        setState(60);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == RegexParser::Comma) {
          setState(56);
          match(RegexParser::Comma);
          setState(58);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == RegexParser::INT) {
            setState(57);
            antlrcpp::downCast<QuantifierContext *>(_localctx)->max = number();
          }
        }
        setState(62);
        match(RegexParser::CloseBrace);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

RegexParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> RegexParser::NumberContext::INT() {
  return getTokens(RegexParser::INT);
}

tree::TerminalNode* RegexParser::NumberContext::INT(size_t i) {
  return getToken(RegexParser::INT, i);
}


size_t RegexParser::NumberContext::getRuleIndex() const {
  return RegexParser::RuleNumber;
}


antlrcpp::Any RegexParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::NumberContext* RegexParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 10, RegexParser::RuleNumber);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(67); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(66);
      match(RegexParser::INT);
      setState(69); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == RegexParser::INT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

RegexParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RegexParser::CharacterContext* RegexParser::AtomContext::character() {
  return getRuleContext<RegexParser::CharacterContext>(0);
}

RegexParser::CharacterClassContext* RegexParser::AtomContext::characterClass() {
  return getRuleContext<RegexParser::CharacterClassContext>(0);
}

tree::TerminalNode* RegexParser::AtomContext::Wildcard() {
  return getToken(RegexParser::Wildcard, 0);
}

tree::TerminalNode* RegexParser::AtomContext::OpenParen() {
  return getToken(RegexParser::OpenParen, 0);
}

tree::TerminalNode* RegexParser::AtomContext::CloseParen() {
  return getToken(RegexParser::CloseParen, 0);
}

RegexParser::AlternationContext* RegexParser::AtomContext::alternation() {
  return getRuleContext<RegexParser::AlternationContext>(0);
}


size_t RegexParser::AtomContext::getRuleIndex() const {
  return RegexParser::RuleAtom;
}


antlrcpp::Any RegexParser::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::AtomContext* RegexParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 12, RegexParser::RuleAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(78);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RegexParser::INT:
      case RegexParser::EscapeChar:
      case RegexParser::LETTER: {
        enterOuterAlt(_localctx, 1);
        setState(71);
        character();
        break;
      }

      case RegexParser::CharacterClassStart:
      case RegexParser::Underscore: {
        enterOuterAlt(_localctx, 2);
        setState(72);
        characterClass();
        break;
      }

      case RegexParser::Wildcard: {
        enterOuterAlt(_localctx, 3);
        setState(73);
        match(RegexParser::Wildcard);
        break;
      }

      case RegexParser::OpenParen: {
        enterOuterAlt(_localctx, 4);
        setState(74);
        match(RegexParser::OpenParen);
        setState(75);
        antlrcpp::downCast<AtomContext *>(_localctx)->capture = alternation();
        setState(76);
        match(RegexParser::CloseParen);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CharacterContext ------------------------------------------------------------------

RegexParser::CharacterContext::CharacterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RegexParser::RegularCharacterContext* RegexParser::CharacterContext::regularCharacter() {
  return getRuleContext<RegexParser::RegularCharacterContext>(0);
}

tree::TerminalNode* RegexParser::CharacterContext::EscapeChar() {
  return getToken(RegexParser::EscapeChar, 0);
}

RegexParser::SpecialCharContext* RegexParser::CharacterContext::specialChar() {
  return getRuleContext<RegexParser::SpecialCharContext>(0);
}


size_t RegexParser::CharacterContext::getRuleIndex() const {
  return RegexParser::RuleCharacter;
}


antlrcpp::Any RegexParser::CharacterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitCharacter(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::CharacterContext* RegexParser::character() {
  CharacterContext *_localctx = _tracker.createInstance<CharacterContext>(_ctx, getState());
  enterRule(_localctx, 14, RegexParser::RuleCharacter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(83);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RegexParser::INT:
      case RegexParser::LETTER: {
        enterOuterAlt(_localctx, 1);
        setState(80);
        regularCharacter();
        break;
      }

      case RegexParser::EscapeChar: {
        enterOuterAlt(_localctx, 2);
        setState(81);
        match(RegexParser::EscapeChar);
        setState(82);
        specialChar();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CharacterClassContext ------------------------------------------------------------------

RegexParser::CharacterClassContext::CharacterClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t RegexParser::CharacterClassContext::getRuleIndex() const {
  return RegexParser::RuleCharacterClass;
}

void RegexParser::CharacterClassContext::copyFrom(CharacterClassContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CcPositiveContext ------------------------------------------------------------------

tree::TerminalNode* RegexParser::CcPositiveContext::CharacterClassStart() {
  return getToken(RegexParser::CharacterClassStart, 0);
}

tree::TerminalNode* RegexParser::CcPositiveContext::CharacterClassEnd() {
  return getToken(RegexParser::CharacterClassEnd, 0);
}

std::vector<RegexParser::ClassMemberContext *> RegexParser::CcPositiveContext::classMember() {
  return getRuleContexts<RegexParser::ClassMemberContext>();
}

RegexParser::ClassMemberContext* RegexParser::CcPositiveContext::classMember(size_t i) {
  return getRuleContext<RegexParser::ClassMemberContext>(i);
}

RegexParser::CcPositiveContext::CcPositiveContext(CharacterClassContext *ctx) { copyFrom(ctx); }


antlrcpp::Any RegexParser::CcPositiveContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitCcPositive(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AnyCharContext ------------------------------------------------------------------

tree::TerminalNode* RegexParser::AnyCharContext::Underscore() {
  return getToken(RegexParser::Underscore, 0);
}

RegexParser::AnyCharContext::AnyCharContext(CharacterClassContext *ctx) { copyFrom(ctx); }


antlrcpp::Any RegexParser::AnyCharContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitAnyChar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CcNegativeContext ------------------------------------------------------------------

tree::TerminalNode* RegexParser::CcNegativeContext::CharacterClassStart() {
  return getToken(RegexParser::CharacterClassStart, 0);
}

tree::TerminalNode* RegexParser::CcNegativeContext::Caret() {
  return getToken(RegexParser::Caret, 0);
}

tree::TerminalNode* RegexParser::CcNegativeContext::CharacterClassEnd() {
  return getToken(RegexParser::CharacterClassEnd, 0);
}

std::vector<RegexParser::ClassMemberContext *> RegexParser::CcNegativeContext::classMember() {
  return getRuleContexts<RegexParser::ClassMemberContext>();
}

RegexParser::ClassMemberContext* RegexParser::CcNegativeContext::classMember(size_t i) {
  return getRuleContext<RegexParser::ClassMemberContext>(i);
}

RegexParser::CcNegativeContext::CcNegativeContext(CharacterClassContext *ctx) { copyFrom(ctx); }


antlrcpp::Any RegexParser::CcNegativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitCcNegative(this);
  else
    return visitor->visitChildren(this);
}
RegexParser::CharacterClassContext* RegexParser::characterClass() {
  CharacterClassContext *_localctx = _tracker.createInstance<CharacterClassContext>(_ctx, getState());
  enterRule(_localctx, 16, RegexParser::RuleCharacterClass);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(103);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<RegexParser::AnyCharContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(85);
      match(RegexParser::Underscore);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<RegexParser::CcPositiveContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(86);
      match(RegexParser::CharacterClassStart);
      setState(88); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(87);
        antlrcpp::downCast<CcPositiveContext *>(_localctx)->positive = classMember();
        setState(90); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << RegexParser::T__0)
        | (1ULL << RegexParser::INT)
        | (1ULL << RegexParser::EscapeChar)
        | (1ULL << RegexParser::LETTER))) != 0));
      setState(92);
      match(RegexParser::CharacterClassEnd);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<RegexParser::CcNegativeContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(94);
      match(RegexParser::CharacterClassStart);
      setState(95);
      match(RegexParser::Caret);
      setState(97); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(96);
        antlrcpp::downCast<CcNegativeContext *>(_localctx)->negative = classMember();
        setState(99); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << RegexParser::T__0)
        | (1ULL << RegexParser::INT)
        | (1ULL << RegexParser::EscapeChar)
        | (1ULL << RegexParser::LETTER))) != 0));
      setState(101);
      match(RegexParser::CharacterClassEnd);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassMemberContext ------------------------------------------------------------------

RegexParser::ClassMemberContext::ClassMemberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RegexParser::CharacterContext* RegexParser::ClassMemberContext::character() {
  return getRuleContext<RegexParser::CharacterContext>(0);
}

RegexParser::RangeContext* RegexParser::ClassMemberContext::range() {
  return getRuleContext<RegexParser::RangeContext>(0);
}

RegexParser::PredefinedClassContext* RegexParser::ClassMemberContext::predefinedClass() {
  return getRuleContext<RegexParser::PredefinedClassContext>(0);
}


size_t RegexParser::ClassMemberContext::getRuleIndex() const {
  return RegexParser::RuleClassMember;
}


antlrcpp::Any RegexParser::ClassMemberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitClassMember(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::ClassMemberContext* RegexParser::classMember() {
  ClassMemberContext *_localctx = _tracker.createInstance<ClassMemberContext>(_ctx, getState());
  enterRule(_localctx, 18, RegexParser::RuleClassMember);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(108);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(105);
      character();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(106);
      range();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(107);
      predefinedClass();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RangeContext ------------------------------------------------------------------

RegexParser::RangeContext::RangeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RegexParser::RangeContext::Hyphen() {
  return getToken(RegexParser::Hyphen, 0);
}

std::vector<RegexParser::CharacterContext *> RegexParser::RangeContext::character() {
  return getRuleContexts<RegexParser::CharacterContext>();
}

RegexParser::CharacterContext* RegexParser::RangeContext::character(size_t i) {
  return getRuleContext<RegexParser::CharacterContext>(i);
}


size_t RegexParser::RangeContext::getRuleIndex() const {
  return RegexParser::RuleRange;
}


antlrcpp::Any RegexParser::RangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitRange(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::RangeContext* RegexParser::range() {
  RangeContext *_localctx = _tracker.createInstance<RangeContext>(_ctx, getState());
  enterRule(_localctx, 20, RegexParser::RuleRange);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(110);
    antlrcpp::downCast<RangeContext *>(_localctx)->min = character();
    setState(111);
    match(RegexParser::Hyphen);
    setState(112);
    antlrcpp::downCast<RangeContext *>(_localctx)->max = character();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PredefinedClassContext ------------------------------------------------------------------

RegexParser::PredefinedClassContext::PredefinedClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RegexParser::PredefinedClassNameContext* RegexParser::PredefinedClassContext::predefinedClassName() {
  return getRuleContext<RegexParser::PredefinedClassNameContext>(0);
}


size_t RegexParser::PredefinedClassContext::getRuleIndex() const {
  return RegexParser::RulePredefinedClass;
}


antlrcpp::Any RegexParser::PredefinedClassContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitPredefinedClass(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::PredefinedClassContext* RegexParser::predefinedClass() {
  PredefinedClassContext *_localctx = _tracker.createInstance<PredefinedClassContext>(_ctx, getState());
  enterRule(_localctx, 22, RegexParser::RulePredefinedClass);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(114);
    match(RegexParser::T__0);
    setState(115);
    predefinedClassName();
    setState(116);
    match(RegexParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PredefinedClassNameContext ------------------------------------------------------------------

RegexParser::PredefinedClassNameContext::PredefinedClassNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t RegexParser::PredefinedClassNameContext::getRuleIndex() const {
  return RegexParser::RulePredefinedClassName;
}


antlrcpp::Any RegexParser::PredefinedClassNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitPredefinedClassName(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::PredefinedClassNameContext* RegexParser::predefinedClassName() {
  PredefinedClassNameContext *_localctx = _tracker.createInstance<PredefinedClassNameContext>(_ctx, getState());
  enterRule(_localctx, 24, RegexParser::RulePredefinedClassName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(125);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RegexParser::T__2: {
        enterOuterAlt(_localctx, 1);
        setState(118);
        antlrcpp::downCast<PredefinedClassNameContext *>(_localctx)->value = match(RegexParser::T__2);
        break;
      }

      case RegexParser::T__3: {
        enterOuterAlt(_localctx, 2);
        setState(119);
        antlrcpp::downCast<PredefinedClassNameContext *>(_localctx)->value = match(RegexParser::T__3);
        break;
      }

      case RegexParser::T__4: {
        enterOuterAlt(_localctx, 3);
        setState(120);
        antlrcpp::downCast<PredefinedClassNameContext *>(_localctx)->value = match(RegexParser::T__4);
        break;
      }

      case RegexParser::T__5: {
        enterOuterAlt(_localctx, 4);
        setState(121);
        antlrcpp::downCast<PredefinedClassNameContext *>(_localctx)->value = match(RegexParser::T__5);
        break;
      }

      case RegexParser::T__6: {
        enterOuterAlt(_localctx, 5);
        setState(122);
        antlrcpp::downCast<PredefinedClassNameContext *>(_localctx)->value = match(RegexParser::T__6);
        break;
      }

      case RegexParser::T__7: {
        enterOuterAlt(_localctx, 6);
        setState(123);
        antlrcpp::downCast<PredefinedClassNameContext *>(_localctx)->value = match(RegexParser::T__7);
        break;
      }

      case RegexParser::T__8: {
        enterOuterAlt(_localctx, 7);
        setState(124);
        antlrcpp::downCast<PredefinedClassNameContext *>(_localctx)->value = match(RegexParser::T__8);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RegularCharacterContext ------------------------------------------------------------------

RegexParser::RegularCharacterContext::RegularCharacterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RegexParser::RegularCharacterContext::LETTER() {
  return getToken(RegexParser::LETTER, 0);
}

tree::TerminalNode* RegexParser::RegularCharacterContext::INT() {
  return getToken(RegexParser::INT, 0);
}


size_t RegexParser::RegularCharacterContext::getRuleIndex() const {
  return RegexParser::RuleRegularCharacter;
}


antlrcpp::Any RegexParser::RegularCharacterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitRegularCharacter(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::RegularCharacterContext* RegexParser::regularCharacter() {
  RegularCharacterContext *_localctx = _tracker.createInstance<RegularCharacterContext>(_ctx, getState());
  enterRule(_localctx, 26, RegexParser::RuleRegularCharacter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(129);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RegexParser::LETTER: {
        enterOuterAlt(_localctx, 1);
        setState(127);
        antlrcpp::downCast<RegularCharacterContext *>(_localctx)->value = match(RegexParser::LETTER);
        break;
      }

      case RegexParser::INT: {
        enterOuterAlt(_localctx, 2);
        setState(128);
        antlrcpp::downCast<RegularCharacterContext *>(_localctx)->value = match(RegexParser::INT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SpecialCharContext ------------------------------------------------------------------

RegexParser::SpecialCharContext::SpecialCharContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RegexParser::SpecialCharContext::Asterisk() {
  return getToken(RegexParser::Asterisk, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::Plus() {
  return getToken(RegexParser::Plus, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::Qmark() {
  return getToken(RegexParser::Qmark, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::CharacterClassStart() {
  return getToken(RegexParser::CharacterClassStart, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::CharacterClassEnd() {
  return getToken(RegexParser::CharacterClassEnd, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::OpenParen() {
  return getToken(RegexParser::OpenParen, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::CloseParen() {
  return getToken(RegexParser::CloseParen, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::Caret() {
  return getToken(RegexParser::Caret, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::Hyphen() {
  return getToken(RegexParser::Hyphen, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::Underscore() {
  return getToken(RegexParser::Underscore, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::Pipe() {
  return getToken(RegexParser::Pipe, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::OpenBrace() {
  return getToken(RegexParser::OpenBrace, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::CloseBrace() {
  return getToken(RegexParser::CloseBrace, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::Wildcard() {
  return getToken(RegexParser::Wildcard, 0);
}

tree::TerminalNode* RegexParser::SpecialCharContext::EscapeChar() {
  return getToken(RegexParser::EscapeChar, 0);
}


size_t RegexParser::SpecialCharContext::getRuleIndex() const {
  return RegexParser::RuleSpecialChar;
}


antlrcpp::Any RegexParser::SpecialCharContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RegexVisitor*>(visitor))
    return parserVisitor->visitSpecialChar(this);
  else
    return visitor->visitChildren(this);
}

RegexParser::SpecialCharContext* RegexParser::specialChar() {
  SpecialCharContext *_localctx = _tracker.createInstance<SpecialCharContext>(_ctx, getState());
  enterRule(_localctx, 28, RegexParser::RuleSpecialChar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(146);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RegexParser::Asterisk: {
        enterOuterAlt(_localctx, 1);
        setState(131);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Asterisk);
        break;
      }

      case RegexParser::Plus: {
        enterOuterAlt(_localctx, 2);
        setState(132);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Plus);
        break;
      }

      case RegexParser::Qmark: {
        enterOuterAlt(_localctx, 3);
        setState(133);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Qmark);
        break;
      }

      case RegexParser::CharacterClassStart: {
        enterOuterAlt(_localctx, 4);
        setState(134);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::CharacterClassStart);
        break;
      }

      case RegexParser::CharacterClassEnd: {
        enterOuterAlt(_localctx, 5);
        setState(135);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::CharacterClassEnd);
        break;
      }

      case RegexParser::OpenParen: {
        enterOuterAlt(_localctx, 6);
        setState(136);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::OpenParen);
        break;
      }

      case RegexParser::CloseParen: {
        enterOuterAlt(_localctx, 7);
        setState(137);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::CloseParen);
        break;
      }

      case RegexParser::Caret: {
        enterOuterAlt(_localctx, 8);
        setState(138);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Caret);
        break;
      }

      case RegexParser::Hyphen: {
        enterOuterAlt(_localctx, 9);
        setState(139);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Hyphen);
        break;
      }

      case RegexParser::Underscore: {
        enterOuterAlt(_localctx, 10);
        setState(140);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Underscore);
        break;
      }

      case RegexParser::Pipe: {
        enterOuterAlt(_localctx, 11);
        setState(141);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Pipe);
        break;
      }

      case RegexParser::OpenBrace: {
        enterOuterAlt(_localctx, 12);
        setState(142);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::OpenBrace);
        break;
      }

      case RegexParser::CloseBrace: {
        enterOuterAlt(_localctx, 13);
        setState(143);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::CloseBrace);
        break;
      }

      case RegexParser::Wildcard: {
        enterOuterAlt(_localctx, 14);
        setState(144);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::Wildcard);
        break;
      }

      case RegexParser::EscapeChar: {
        enterOuterAlt(_localctx, 15);
        setState(145);
        antlrcpp::downCast<SpecialCharContext *>(_localctx)->value = match(RegexParser::EscapeChar);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> RegexParser::_decisionToDFA;
atn::PredictionContextCache RegexParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN RegexParser::_atn;
std::vector<uint16_t> RegexParser::_serializedATN;

std::vector<std::string> RegexParser::_ruleNames = {
  "regex", "alternation", "expression", "element", "quantifier", "number", 
  "atom", "character", "characterClass", "classMember", "range", "predefinedClass", 
  "predefinedClassName", "regularCharacter", "specialChar"
};

std::vector<std::string> RegexParser::_literalNames = {
  "", "'[:'", "':]'", "'ALPHA'", "'UPPER'", "'LOWER'", "'DIGIT'", "'ALNUM'", 
  "'SPACE'", "'WHITESPACE'", "", "", "'%'", "'['", "']'", "'^'", "'-'", 
  "'_'", "'|'", "'('", "')'", "'+'", "'*'", "'\u003F'", "'{'", "'}'", "','", 
  "'\\'"
};

std::vector<std::string> RegexParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "WS", "INT", "Wildcard", "CharacterClassStart", 
  "CharacterClassEnd", "Caret", "Hyphen", "Underscore", "Pipe", "OpenParen", 
  "CloseParen", "Plus", "Asterisk", "Qmark", "OpenBrace", "CloseBrace", 
  "Comma", "EscapeChar", "LETTER"
};

dfa::Vocabulary RegexParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> RegexParser::_tokenNames;

RegexParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x1e, 0x97, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x3, 0x2, 
       0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x27, 
       0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x2a, 0xb, 0x3, 0x3, 0x4, 0x7, 0x4, 
       0x2d, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x30, 0xb, 0x4, 0x3, 0x5, 0x3, 
       0x5, 0x5, 0x5, 0x34, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x3d, 0xa, 0x6, 0x5, 
       0x6, 0x3f, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x43, 0xa, 0x6, 
       0x3, 0x7, 0x6, 0x7, 0x46, 0xa, 0x7, 0xd, 0x7, 0xe, 0x7, 0x47, 0x3, 
       0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
       0x5, 0x8, 0x51, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 
       0x56, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x6, 0xa, 0x5b, 0xa, 
       0xa, 0xd, 0xa, 0xe, 0xa, 0x5c, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xa, 0x6, 0xa, 0x64, 0xa, 0xa, 0xd, 0xa, 0xe, 0xa, 0x65, 
       0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x6a, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 
       0x3, 0xb, 0x5, 0xb, 0x6f, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
       0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 
       0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 
       0x80, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x84, 0xa, 0xf, 0x3, 
       0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
       0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
       0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x95, 0xa, 0x10, 0x3, 0x10, 
       0x2, 0x2, 0x11, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 
       0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x2, 0x2, 0x2, 0xaf, 0x2, 0x20, 0x3, 
       0x2, 0x2, 0x2, 0x4, 0x23, 0x3, 0x2, 0x2, 0x2, 0x6, 0x2e, 0x3, 0x2, 
       0x2, 0x2, 0x8, 0x31, 0x3, 0x2, 0x2, 0x2, 0xa, 0x42, 0x3, 0x2, 0x2, 
       0x2, 0xc, 0x45, 0x3, 0x2, 0x2, 0x2, 0xe, 0x50, 0x3, 0x2, 0x2, 0x2, 
       0x10, 0x55, 0x3, 0x2, 0x2, 0x2, 0x12, 0x69, 0x3, 0x2, 0x2, 0x2, 0x14, 
       0x6e, 0x3, 0x2, 0x2, 0x2, 0x16, 0x70, 0x3, 0x2, 0x2, 0x2, 0x18, 0x74, 
       0x3, 0x2, 0x2, 0x2, 0x1a, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x83, 0x3, 
       0x2, 0x2, 0x2, 0x1e, 0x94, 0x3, 0x2, 0x2, 0x2, 0x20, 0x21, 0x5, 0x4, 
       0x3, 0x2, 0x21, 0x22, 0x7, 0x2, 0x2, 0x3, 0x22, 0x3, 0x3, 0x2, 0x2, 
       0x2, 0x23, 0x28, 0x5, 0x6, 0x4, 0x2, 0x24, 0x25, 0x7, 0x14, 0x2, 
       0x2, 0x25, 0x27, 0x5, 0x6, 0x4, 0x2, 0x26, 0x24, 0x3, 0x2, 0x2, 0x2, 
       0x27, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x28, 0x26, 0x3, 0x2, 0x2, 0x2, 0x28, 
       0x29, 0x3, 0x2, 0x2, 0x2, 0x29, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x28, 
       0x3, 0x2, 0x2, 0x2, 0x2b, 0x2d, 0x5, 0x8, 0x5, 0x2, 0x2c, 0x2b, 0x3, 
       0x2, 0x2, 0x2, 0x2d, 0x30, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x2c, 0x3, 0x2, 
       0x2, 0x2, 0x2e, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x7, 0x3, 0x2, 0x2, 
       0x2, 0x30, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x31, 0x33, 0x5, 0xe, 0x8, 0x2, 
       0x32, 0x34, 0x5, 0xa, 0x6, 0x2, 0x33, 0x32, 0x3, 0x2, 0x2, 0x2, 0x33, 
       0x34, 0x3, 0x2, 0x2, 0x2, 0x34, 0x9, 0x3, 0x2, 0x2, 0x2, 0x35, 0x43, 
       0x7, 0x18, 0x2, 0x2, 0x36, 0x43, 0x7, 0x17, 0x2, 0x2, 0x37, 0x43, 
       0x7, 0x19, 0x2, 0x2, 0x38, 0x39, 0x7, 0x1a, 0x2, 0x2, 0x39, 0x3e, 
       0x5, 0xc, 0x7, 0x2, 0x3a, 0x3c, 0x7, 0x1c, 0x2, 0x2, 0x3b, 0x3d, 
       0x5, 0xc, 0x7, 0x2, 0x3c, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x3d, 0x3, 
       0x2, 0x2, 0x2, 0x3d, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x3a, 0x3, 0x2, 
       0x2, 0x2, 0x3e, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x40, 0x3, 0x2, 0x2, 
       0x2, 0x40, 0x41, 0x7, 0x1b, 0x2, 0x2, 0x41, 0x43, 0x3, 0x2, 0x2, 
       0x2, 0x42, 0x35, 0x3, 0x2, 0x2, 0x2, 0x42, 0x36, 0x3, 0x2, 0x2, 0x2, 
       0x42, 0x37, 0x3, 0x2, 0x2, 0x2, 0x42, 0x38, 0x3, 0x2, 0x2, 0x2, 0x43, 
       0xb, 0x3, 0x2, 0x2, 0x2, 0x44, 0x46, 0x7, 0xd, 0x2, 0x2, 0x45, 0x44, 
       0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x3, 0x2, 0x2, 0x2, 0x47, 0x45, 0x3, 
       0x2, 0x2, 0x2, 0x47, 0x48, 0x3, 0x2, 0x2, 0x2, 0x48, 0xd, 0x3, 0x2, 
       0x2, 0x2, 0x49, 0x51, 0x5, 0x10, 0x9, 0x2, 0x4a, 0x51, 0x5, 0x12, 
       0xa, 0x2, 0x4b, 0x51, 0x7, 0xe, 0x2, 0x2, 0x4c, 0x4d, 0x7, 0x15, 
       0x2, 0x2, 0x4d, 0x4e, 0x5, 0x4, 0x3, 0x2, 0x4e, 0x4f, 0x7, 0x16, 
       0x2, 0x2, 0x4f, 0x51, 0x3, 0x2, 0x2, 0x2, 0x50, 0x49, 0x3, 0x2, 0x2, 
       0x2, 0x50, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x50, 0x4b, 0x3, 0x2, 0x2, 0x2, 
       0x50, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x51, 0xf, 0x3, 0x2, 0x2, 0x2, 0x52, 
       0x56, 0x5, 0x1c, 0xf, 0x2, 0x53, 0x54, 0x7, 0x1d, 0x2, 0x2, 0x54, 
       0x56, 0x5, 0x1e, 0x10, 0x2, 0x55, 0x52, 0x3, 0x2, 0x2, 0x2, 0x55, 
       0x53, 0x3, 0x2, 0x2, 0x2, 0x56, 0x11, 0x3, 0x2, 0x2, 0x2, 0x57, 0x6a, 
       0x7, 0x13, 0x2, 0x2, 0x58, 0x5a, 0x7, 0xf, 0x2, 0x2, 0x59, 0x5b, 
       0x5, 0x14, 0xb, 0x2, 0x5a, 0x59, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x5c, 
       0x3, 0x2, 0x2, 0x2, 0x5c, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 0x3, 
       0x2, 0x2, 0x2, 0x5d, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x5f, 0x7, 0x10, 
       0x2, 0x2, 0x5f, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x60, 0x61, 0x7, 0xf, 0x2, 
       0x2, 0x61, 0x63, 0x7, 0x11, 0x2, 0x2, 0x62, 0x64, 0x5, 0x14, 0xb, 
       0x2, 0x63, 0x62, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 0x3, 0x2, 0x2, 0x2, 
       0x65, 0x63, 0x3, 0x2, 0x2, 0x2, 0x65, 0x66, 0x3, 0x2, 0x2, 0x2, 0x66, 
       0x67, 0x3, 0x2, 0x2, 0x2, 0x67, 0x68, 0x7, 0x10, 0x2, 0x2, 0x68, 
       0x6a, 0x3, 0x2, 0x2, 0x2, 0x69, 0x57, 0x3, 0x2, 0x2, 0x2, 0x69, 0x58, 
       0x3, 0x2, 0x2, 0x2, 0x69, 0x60, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x13, 0x3, 
       0x2, 0x2, 0x2, 0x6b, 0x6f, 0x5, 0x10, 0x9, 0x2, 0x6c, 0x6f, 0x5, 
       0x16, 0xc, 0x2, 0x6d, 0x6f, 0x5, 0x18, 0xd, 0x2, 0x6e, 0x6b, 0x3, 
       0x2, 0x2, 0x2, 0x6e, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6d, 0x3, 0x2, 
       0x2, 0x2, 0x6f, 0x15, 0x3, 0x2, 0x2, 0x2, 0x70, 0x71, 0x5, 0x10, 
       0x9, 0x2, 0x71, 0x72, 0x7, 0x12, 0x2, 0x2, 0x72, 0x73, 0x5, 0x10, 
       0x9, 0x2, 0x73, 0x17, 0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0x7, 0x3, 0x2, 
       0x2, 0x75, 0x76, 0x5, 0x1a, 0xe, 0x2, 0x76, 0x77, 0x7, 0x4, 0x2, 
       0x2, 0x77, 0x19, 0x3, 0x2, 0x2, 0x2, 0x78, 0x80, 0x7, 0x5, 0x2, 0x2, 
       0x79, 0x80, 0x7, 0x6, 0x2, 0x2, 0x7a, 0x80, 0x7, 0x7, 0x2, 0x2, 0x7b, 
       0x80, 0x7, 0x8, 0x2, 0x2, 0x7c, 0x80, 0x7, 0x9, 0x2, 0x2, 0x7d, 0x80, 
       0x7, 0xa, 0x2, 0x2, 0x7e, 0x80, 0x7, 0xb, 0x2, 0x2, 0x7f, 0x78, 0x3, 
       0x2, 0x2, 0x2, 0x7f, 0x79, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x7a, 0x3, 0x2, 
       0x2, 0x2, 0x7f, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x7c, 0x3, 0x2, 0x2, 
       0x2, 0x7f, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x7e, 0x3, 0x2, 0x2, 0x2, 
       0x80, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x81, 0x84, 0x7, 0x1e, 0x2, 0x2, 
       0x82, 0x84, 0x7, 0xd, 0x2, 0x2, 0x83, 0x81, 0x3, 0x2, 0x2, 0x2, 0x83, 
       0x82, 0x3, 0x2, 0x2, 0x2, 0x84, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x85, 0x95, 
       0x7, 0x18, 0x2, 0x2, 0x86, 0x95, 0x7, 0x17, 0x2, 0x2, 0x87, 0x95, 
       0x7, 0x19, 0x2, 0x2, 0x88, 0x95, 0x7, 0xf, 0x2, 0x2, 0x89, 0x95, 
       0x7, 0x10, 0x2, 0x2, 0x8a, 0x95, 0x7, 0x15, 0x2, 0x2, 0x8b, 0x95, 
       0x7, 0x16, 0x2, 0x2, 0x8c, 0x95, 0x7, 0x11, 0x2, 0x2, 0x8d, 0x95, 
       0x7, 0x12, 0x2, 0x2, 0x8e, 0x95, 0x7, 0x13, 0x2, 0x2, 0x8f, 0x95, 
       0x7, 0x14, 0x2, 0x2, 0x90, 0x95, 0x7, 0x1a, 0x2, 0x2, 0x91, 0x95, 
       0x7, 0x1b, 0x2, 0x2, 0x92, 0x95, 0x7, 0xe, 0x2, 0x2, 0x93, 0x95, 
       0x7, 0x1d, 0x2, 0x2, 0x94, 0x85, 0x3, 0x2, 0x2, 0x2, 0x94, 0x86, 
       0x3, 0x2, 0x2, 0x2, 0x94, 0x87, 0x3, 0x2, 0x2, 0x2, 0x94, 0x88, 0x3, 
       0x2, 0x2, 0x2, 0x94, 0x89, 0x3, 0x2, 0x2, 0x2, 0x94, 0x8a, 0x3, 0x2, 
       0x2, 0x2, 0x94, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x94, 0x8c, 0x3, 0x2, 0x2, 
       0x2, 0x94, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x94, 0x8e, 0x3, 0x2, 0x2, 0x2, 
       0x94, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x94, 0x90, 0x3, 0x2, 0x2, 0x2, 0x94, 
       0x91, 0x3, 0x2, 0x2, 0x2, 0x94, 0x92, 0x3, 0x2, 0x2, 0x2, 0x94, 0x93, 
       0x3, 0x2, 0x2, 0x2, 0x95, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x12, 0x28, 0x2e, 
       0x33, 0x3c, 0x3e, 0x42, 0x47, 0x50, 0x55, 0x5c, 0x65, 0x69, 0x6e, 
       0x7f, 0x83, 0x94, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

RegexParser::Initializer RegexParser::_init;
