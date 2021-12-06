#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class ConcatNode : public IBaseNode {
private:
  IBaseNode* lhs;
  IBaseNode* rhs;

public:
  ConcatNode(IBaseNode* lhs, IBaseNode* rhs) : lhs(lhs), rhs(rhs) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};