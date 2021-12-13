#pragma once

#include <parser/RegExpHelpers.h>
#include <parser/nodes/IBaseNode.h>
#include <stdint.h>

#include <string>

class RegExp {
private:
  /**
   * @brief: Syntax flag, enables all optional regexp syntax.
   */
  static const uint32_t ALL = 0xffff;

  /**
   * @brief: Syntax flag, disables all optional regexp syntax.
   */
  static const uint32_t NONE = 0x0000;

  /**
   * @brief Syntax flags for regexp syntax. Defaults to ALL.
   */
  const uint32_t compiler_flags;

  /**
   * @brief Original Regular Expression.
   */
  const std::string regexp;

  int32_t pos = 0;

  /**
   * Parsing Methods.
   */
  IBaseNode* parse_union_expr();
  IBaseNode* parse_concat_expr();
  IBaseNode* parse_repeat_expr();
  IBaseNode* parse_char_class_expr();
  IBaseNode* parse_char_classes();
  IBaseNode* parse_char_class();
  IBaseNode* parse_simple_expr();
  char parse_char_expr();

  /**
   * Helpers.
   */
  bool peek(const std::string& str);
  bool match(char c);
  bool has_more();
  char next();
  bool check(uint32_t flag) const;

public:
  std::string curr_regexp;

  uint32_t min, max;
  IBaseNode* lhs;
  IBaseNode* rhs;

  explicit RegExp(const std::string& regexp) : RegExp(regexp, ALL) {}
  RegExp() : RegExp("", NONE) {}
  RegExp(const std::string& regexp, u_int32_t flags) : compiler_flags(flags), regexp(regexp) {}

  IBaseNode* parse();
};