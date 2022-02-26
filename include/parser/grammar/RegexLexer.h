
// Generated from /Users/melzarei/Documents/code-compiling-regex/include/parser/grammar/Regex.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  RegexLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, WS = 10, INT = 11, Wildcard = 12, CharacterClassStart = 13, 
    CharacterClassEnd = 14, Caret = 15, Hyphen = 16, Underscore = 17, Pipe = 18, 
    OpenParen = 19, CloseParen = 20, Plus = 21, Asterisk = 22, Qmark = 23, 
    OpenBrace = 24, CloseBrace = 25, Comma = 26, EscapeChar = 27, LETTER = 28
  };

  explicit RegexLexer(antlr4::CharStream *input);
  ~RegexLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

