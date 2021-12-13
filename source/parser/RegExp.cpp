#include <fmt/core.h>
#include <parser/RegExp.h>

IBaseNode* RegExp::parse_union_expr() {
  IBaseNode* e = parse_concat_expr();
  if (match('|')) {
    e = RegExpHelpers::make_union(e, parse_union_expr());
  }
  return e;
}

IBaseNode* RegExp::parse_concat_expr() {
  IBaseNode* e = parse_repeat_expr();
  if (has_more() && !peek(")|")) {
    e = RegExpHelpers::make_concat(e, parse_concat_expr());
  }
  return e;
}

IBaseNode* RegExp::parse_repeat_expr() {
  IBaseNode* e = parse_char_class_expr();
  while (peek("?*+{")) {
    if (match('?')) {
      e = RegExpHelpers::make_optional(e);
    } else if (match('*')) {
      e = RegExpHelpers::make_repeat(e);
    } else if (match('+')) {
      e = RegExpHelpers::make_repeat(e, 1);
    } else if (match('{')) {
      int start = pos;
      while (peek("0123456789")) next();
      if (start == pos)
        throw std::runtime_error(fmt::format("Integer expected at position {}. ", pos));
      int n = std::stoi(std::string(regexp, start, pos));
      int m = -1;
      if (match(',')) {
        start = pos;
        while (peek("0123456789")) next();
        if (start != pos) m = std::stoi(std::string(regexp, start, pos));
      } else
        m = n;
      if (!match('}')) throw std::runtime_error(fmt::format("expected \\} at position {}.", pos));
      if (m == -1)
        e = RegExpHelpers::make_repeat(e, n);
      else
        e = RegExpHelpers::make_repeat(e, n, m);
    }
  }
  return e;
}

IBaseNode* RegExp::parse_char_class_expr() {
  if (match('[')) {
    bool negate = false;
    if (match('^')) negate = true;
    IBaseNode* e = parse_char_classes();
    if (negate)
      e = RegExpHelpers::make_intersection(RegExpHelpers::make_any_char(),
                                           RegExpHelpers::make_complement(e));
    if (!match(']')) throw std::runtime_error(fmt::format("expected ']' at position {}. ", pos));
    return e;
  }
  return parse_simple_expr();
}

IBaseNode* RegExp::parse_char_classes() {
  IBaseNode* e = parse_char_class();
  while (has_more() && !peek("]")) e = RegExpHelpers::make_union(e, parse_char_class());
  return e;
}

IBaseNode* RegExp::parse_char_class() {
  char c = parse_char_expr();
  if (match('-')) {
    if (peek("]"))
      return RegExpHelpers::make_union(RegExpHelpers::make_char(c), RegExpHelpers::make_char('-'));
    else
      return RegExpHelpers::make_char_range(c, parse_char_expr());
  } else {
    return RegExpHelpers::make_char(c);
  }
}

IBaseNode* RegExp::parse_simple_expr() {
  if (match('.'))
    return RegExpHelpers::make_any_char();
  else if (match('"')) {
    uint32_t start = pos;
    while (has_more() && !peek("\"")) next();
    if (!match('"')) throw std::runtime_error(fmt::format("expected '\"' at position {}. ", pos));
    std::string sub(regexp, start, pos - 1);
    return RegExpHelpers::make_string(sub);
  } else if (match('(')) {
    if (match(')')) return RegExpHelpers::make_string("");
    IBaseNode* e = parse_union_expr();
    if (!match(')')) throw std::runtime_error(fmt::format("expected ')' at position {}.", pos));
    return e;
  } else {
    return RegExpHelpers::make_char(parse_char_expr());
  }
}

char RegExp::parse_char_expr() {
  match('\\');
  char nxt = next();
  return nxt;
}

bool RegExp::peek(const std::string& str) {
  return has_more() && str.find(regexp[pos]) != str.npos;
}

bool RegExp::match(char c) {
  if (pos >= regexp.size()) return false;
  if (regexp[pos] == c) {
    pos++;
    return true;
  }
  return false;
}

bool RegExp::has_more() { return pos < regexp.size(); }

char RegExp::next() {
  if (!has_more()) {
    throw std::runtime_error(fmt::format("Unexpected End-of-string."));
  }
  return regexp[pos++];
}

bool RegExp::check(uint32_t flag) const { return (compiler_flags & flag) != 0; }

IBaseNode* RegExp::parse() { return parse_union_expr(); }