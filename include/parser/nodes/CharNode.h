#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class CharNode : public IBaseNode {
private:
  char chr;

public:
  CharNode(char chr) : chr(chr) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};