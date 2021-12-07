#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class UnionNode : public IBaseNode {
private:
  IBaseNode* lhs;
  IBaseNode* rhs;

public:
  UnionNode(IBaseNode* lhs, IBaseNode* rhs) : lhs(lhs), rhs(rhs) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};
