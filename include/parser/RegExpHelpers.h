#include <parser/nodes/AnyCharNode.h>
#include <parser/nodes/AnyStringNode.h>
#include <parser/nodes/AutomataNode.h>
#include <parser/nodes/CharNode.h>
#include <parser/nodes/CharRangeNode.h>
#include <parser/nodes/ComplementNode.h>
#include <parser/nodes/ConcatNode.h>
#include <parser/nodes/EmptyNode.h>
#include <parser/nodes/IntersectionNode.h>
#include <parser/nodes/IntervalNode.h>
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
    // TODO(@melzarei) uncomment the optimization
    // Need to cast

    // if (is_instance(lhs, ConcatNode)) {
    //   // right child of left is string
    //   // and right is is string
    //   if (is_instance(lhs->rhs, StringNode) && is_instance(rhs, StringNode)) {
    //     return new ConcatNode(lhs->lhs, make_string(lhs->rhs, rhs));
    //   }
    // } else if (is_instance(lhs, StringNode) && is_instance(rhs, ConcatNode)
    //            && is_instance(rhs->lhs, StringNode)) {
    //   return new ConcatNode(make_string(lhs, rhs->lhs), rhs);
    // }
    return new ConcatNode(lhs, rhs);
  }

  static ComplementNode* make_complement(IBaseNode* exp) { return new ComplementNode(exp); }

  static IntersectionNode* make_intersection(IBaseNode* lhs, IBaseNode* rhs) {
    return new IntersectionNode(lhs, rhs);
  }

  static StringNode* make_string(std::string s) { return new StringNode(s); }
  static StringNode* make_string(IBaseNode* lhs, IBaseNode* rhs) {
    StringNode* lhs_str = dynamic_cast<StringNode*>(lhs);
    StringNode* rhs_str = dynamic_cast<StringNode*>(rhs);
    auto str = lhs_str->get_str() + rhs_str->get_str();
    auto node = new StringNode(str);

    // free memory
    delete lhs_str;
    delete rhs_str;

    return node;
  }

  static AnyStringNode* make_any_string() { return new AnyStringNode(); }

  static AnyCharNode* make_any_char() { return new AnyCharNode(); }

  static StringNode* make_char(char c) {
    const std::string s(1, c);
    return new StringNode(s);
  }
  static CharRangeNode* make_char_range(char start, char end) {
    return new CharRangeNode(start, end);
  }

  static IntervalNode* make_interval(int32_t min, int32_t max, int32_t digits) {
    return new IntervalNode(min, max, digits);
  }
  static EmptyNode* make_empty() { return new EmptyNode(); }
};
