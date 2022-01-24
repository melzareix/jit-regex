#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class OptionalNode : public IBaseNode {
private:
  IBaseNode* exp_;

public:
  explicit OptionalNode(IBaseNode* exp) : exp_(exp) {}
  IBaseNode* exp() {return exp_;}
  void accept(class IVisitor* v) override { v->visit(this); }
};