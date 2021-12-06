#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class IntersectionNode : public IBaseNode {
private:
  IBaseNode* lhs;
  IBaseNode* rhs;

public:
  IntersectionNode(IBaseNode* lhs, IBaseNode* rhs) : lhs(lhs), rhs(rhs) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};
