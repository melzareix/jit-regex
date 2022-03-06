//
// Created by Mohamed El Zarei on 02/01/2022.
//

#include "fa/transition.h"

#include "fa/state.h"

namespace ZRegex {
  bool operator<(const FiniteAutomatonTransition& t1, const FiniteAutomatonTransition& t2) {
    if (t1.to != t2.to) {
      if (t1.to == nullptr)
        return false;
      else if (t2.to == nullptr)
        return true;
      else if (t1.to->id < t2.to->id)
        return true;
      else if (t1.to->id > t2.to->id)
        return false;
    }

    if (t1.min < t2.min) return true;
    if (t1.min > t2.min) return false;
    if (t1.max > t2.max) return true;
    if (t1.max < t2.max) return false;

    return false;
  }

}  // namespace ZRegex
