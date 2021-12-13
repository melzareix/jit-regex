#pragma once
#include <parser/IVisitor.h>
#include <parser/nodes/IBaseNode.h>

class RepeatNode : public IBaseNode {
private:
  uint32_t min_;
  uint32_t max_;
  IBaseNode* exp_;

public:
  RepeatNode(IBaseNode* exp, uint32_t min, uint32_t max) : exp_(exp), min_(min), max_(max) {}
  void accept(class IVisitor* v) override { v->visit(this); }
  IBaseNode* exp() {return exp_;}

  [[nodiscard]] uint32_t min() const { return min_; }
  [[nodiscard]] uint32_t max() const { return max_; }
};