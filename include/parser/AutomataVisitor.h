#pragma once

#include <parser/IVisitor.h>
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

class AutomataVisitor : public IVisitor {
public:
  void visit(CharRangeNode *v) { v->accept(this); }
  void visit(ComplementNode *v) { v->accept(this); }
  void visit(ConcatNode *v) { v->accept(this); }
  void visit(EmptyNode *v) { v->accept(this); }
  void visit(IntervalNode *v) { v->accept(this); }
  void visit(IntersectionNode *v) { v->accept(this); }
  void visit(OptionalNode *v) { std::cout << "optional" << std::endl; }
  void visit(AnyCharNode *v) { v->accept(this); }
  void visit(AnyStringNode *v) { v->accept(this); }
  void visit(AutomataNode *v) { v->accept(this); }
  void visit(CharNode *v) { v->accept(this); }
  void visit(RepeatNode *v) { std::cout << "repeat: " << std::endl; }
  void visit(StringNode *v) { std::cout << "ss " << v->get_str() << std::endl; }
  void visit(UnionNode *v) { std::cout << "uuuuu" << std::endl; }
};
