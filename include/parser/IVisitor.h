#pragma once

class IVisitor {
public:
  virtual void visit(class CharRangeNode *v) = 0;
  virtual void visit(class ComplementNode *v) = 0;
  virtual void visit(class ConcatNode *v) = 0;
  virtual void visit(class EmptyNode *v) = 0;
  virtual void visit(class IntervalNode *v) = 0;
  virtual void visit(class IntersectionNode *v) = 0;
  virtual void visit(class OptionalNode *v) = 0;
  virtual void visit(class AnyCharNode *v) = 0;
  virtual void visit(class AnyStringNode *v) = 0;
  virtual void visit(class AutomataNode *v) = 0;
  virtual void visit(class CharNode *v) = 0;
  virtual void visit(class RepeatNode *v) = 0;
  virtual void visit(class StringNode *v) = 0;
  virtual void visit(class UnionNode *v) = 0;
};
