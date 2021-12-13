#include <parser/nodes/AnyCharNode.h>
#include <parser/nodes/AutomataNode.h>
#include <parser/nodes/CharNode.h>
#include <parser/nodes/CharRangeNode.h>
#include <parser/nodes/ComplementNode.h>
#include <parser/nodes/ConcatNode.h>
#include <parser/nodes/IntersectionNode.h>
#include <parser/nodes/OptionalNode.h>
#include <parser/nodes/RepeatNode.h>
#include <parser/nodes/StringNode.h>
#include <parser/nodes/UnionNode.h>

#include <iostream>
#include <string>

#define is_instance(ptr, clazz) (dynamic_cast<const clazz*>(ptr) != NULL)

class RegExpHelpers {
public:
  static AutomataNode* make_automata(const std::string& s) { return new AutomataNode(s); }

  static OptionalNode* make_optional(IBaseNode* exp) { return new OptionalNode(exp); }

  static RepeatNode* make_repeat(IBaseNode* exp, uint32_t min = 0, uint32_t max = 0) {
    return new RepeatNode(exp, min, max);
  }

  static UnionNode* make_union(IBaseNode* lhs, IBaseNode* rhs) { return new UnionNode(lhs, rhs); }

  static IBaseNode* make_concat(IBaseNode* lhs, IBaseNode* rhs) {
    if (is_instance(lhs, StringNode) && is_instance(rhs, StringNode)) {
      return make_string(lhs, rhs);
    }

     if (is_instance(lhs, ConcatNode)) {
       auto lhs_cc = dynamic_cast<const ConcatNode*>(lhs);
       // right child of left is string
       // and right is is string
       if (is_instance(lhs_cc->get_rhs(), StringNode) && is_instance(rhs, StringNode)) {
         return new ConcatNode(lhs_cc->get_lhs(), make_string(lhs_cc->get_rhs(), rhs));
       }
     } else if (is_instance(lhs, StringNode) && is_instance(rhs, ConcatNode)) {
       auto rhs_cc = dynamic_cast<const ConcatNode*>(rhs);
       if (is_instance(rhs_cc->get_lhs(), StringNode)) {
         return new ConcatNode(make_string(lhs, rhs_cc->get_lhs()), rhs_cc->get_rhs());
       }
     }
    return new ConcatNode(lhs, rhs);
  }

  static ComplementNode* make_complement(IBaseNode* exp) { return new ComplementNode(exp); }

  static IntersectionNode* make_intersection(IBaseNode* lhs, IBaseNode* rhs) {
    return new IntersectionNode(lhs, rhs);
  }

  static StringNode* make_string(std::string s) { return new StringNode(s); }
  static StringNode* make_string(IBaseNode* lhs, IBaseNode* rhs) {
    auto lhs_str = dynamic_cast<StringNode*>(lhs);
    auto rhs_str = dynamic_cast<StringNode*>(rhs);
    auto str = lhs_str->get_str() + rhs_str->get_str();
    auto node = new StringNode(str);
    return node;
  }

  static AnyCharNode* make_any_char() { return new AnyCharNode(); }

  static StringNode* make_char(char c) {
    const std::string s(1, c);
    return new StringNode(s);
  }
  static CharRangeNode* make_char_range(char start, char end) {
    return new CharRangeNode(start, end);
  }
};
