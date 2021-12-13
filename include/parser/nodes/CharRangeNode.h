#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class CharRangeNode : public IBaseNode {
private:
  char start;
  char end;

public:
  CharRangeNode(char start, char end) : start(start), end(end) {}
  void accept(class IVisitor* v) override { v->visit(this); }
  char get_start() { return start; }
  char get_end() { return end; }
};