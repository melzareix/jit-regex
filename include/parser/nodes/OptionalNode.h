#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class OptionalNode : public IBaseNode {
private:
  IBaseNode* exp;

public:
  OptionalNode(IBaseNode* exp) : exp(exp) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};