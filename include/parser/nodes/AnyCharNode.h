#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class AnyCharNode : public IBaseNode {
public:
  AnyCharNode() = default;
  void accept(class IVisitor* v) override { v->visit(this); }
};