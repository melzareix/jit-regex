#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>
#include <stdint.h>

class IntervalNode : public IBaseNode {
private:
  uint32_t min;
  uint32_t max;
  uint32_t digits;

public:
  IntervalNode(uint32_t min, uint32_t max, uint32_t digits) : min(min), max(max), digits(digits) {}
  void accept(class IVisitor* v) override { v->visit(this); }
};