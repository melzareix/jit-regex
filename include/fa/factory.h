//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_FACTORY_H
#define ZREGEXSTANDALONE_FACTORY_H

#include <memory>
#include <string>
#include <vector>

#include "fa/fa.h"
#include "helpers/utf8.h"

namespace ZRegex {
  struct FAFactory {
    static std::unique_ptr<FiniteAutomaton> AnyChar() {
      return FAFactory::CharRangeAutomaton(0, ZRegex::Utf8::MAX_TRANS);
    }
    static std::unique_ptr<FiniteAutomaton> EpsilonAutomaton() {
      auto p = std::make_shared<FiniteAutomatonState>();
      auto fa = std::make_unique<FiniteAutomaton>(p);
      fa->SetDeterministic(true);
      return std::move(fa);
    }

    static std::unique_ptr<FiniteAutomaton> RepeatMinMax(std::unique_ptr<FiniteAutomaton> a,
                                                         uint32_t min, uint32_t max) {
      max -= min;
      std::vector<std::unique_ptr<FiniteAutomaton>> fas;
      std::unique_ptr<FiniteAutomaton> b;

      if (min == 0) {
        b = FAFactory::EpsilonAutomaton();
      } else if (min == 1) {
        b = std::make_unique<FiniteAutomaton>(*a);
      } else {
        while (min-- > 0) {
          const auto c = *a;
          std::unique_ptr<FiniteAutomaton> cp(new FiniteAutomaton(c));
          fas.emplace_back(std::move(cp));
        }
        b = FAFactory::ConcatAll(fas);
      }

      // max
      if (max > 0) {
        auto d = std::make_unique<FiniteAutomaton>(*a);
        while (--max) {
          auto c = std::make_unique<FiniteAutomaton>(*a);
          for (const auto& s : c->GetAcceptStates()) {
            s->AddEpsilon(d->initial_state);
          }
          d = std::move(c);
        }
        for (const auto& s : b->GetAcceptStates()) {
          s->AddEpsilon(d->initial_state);
        }
        b->SetDeterministic(false);
      }
      return std::move(b);
    }
    // also works for plus operator
    static std::unique_ptr<FiniteAutomaton> RepeatMinimum(std::unique_ptr<FiniteAutomaton> a,
                                                          uint32_t min) {
      if (min == 0) {
        return KleeneStar(std::move(a));
      }
      min++;  //  we need +1 to account for kleene star copy
      std::vector<std::unique_ptr<FiniteAutomaton>> fas;
      fas.reserve(min);

      while (min-- > 0) {
        const auto c = *a;
        std::unique_ptr<FiniteAutomaton> cp(new FiniteAutomaton(c));
        fas.emplace_back(std::move(cp));
      }

      auto kleene = KleeneStar(std::move(fas.back()));
      fas.pop_back();  // remove and add the kleene
      fas.emplace_back(std::move(kleene));
      return ConcatAll(fas);
    }

    static std::unique_ptr<FiniteAutomaton> UnionAll(
        std::vector<std::unique_ptr<FiniteAutomaton>>& fas) {
      auto& prev = fas[0];

      for (int i = 1; i < fas.size(); ++i) {
        prev = Union(std::move(prev), std::move(fas[i]));
      }

      return std::move(prev);
    }

    static std::unique_ptr<FiniteAutomaton> ConcatAll(
        std::vector<std::unique_ptr<FiniteAutomaton>>& fas) {
      auto& prev = fas[0];

      for (int i = 1; i < fas.size(); ++i) {
        prev = Concat(std::move(prev), std::move(fas[i]));
      }

      return std::move(prev);
    }
    static std::unique_ptr<FiniteAutomaton> Optional(std::unique_ptr<FiniteAutomaton> a) {
      auto p = std::make_shared<FiniteAutomatonState>();
      p->SetAccept(true);
      p->AddEpsilon(a->initial_state);
      a->SetDeterministic(false);
      a->SetInitial(p);
      return a;
    }
    static std::unique_ptr<FiniteAutomaton> KleeneStar(std::unique_ptr<FiniteAutomaton> a) {
      auto p = std::make_shared<FiniteAutomatonState>();
      p->SetAccept(true);
      p->AddEpsilon(a->initial_state);
      auto ac = a->GetAcceptStates();
      for (const auto& s : ac) {
        s->AddEpsilon(p);
      }
      a->SetDeterministic(false);
      a->SetInitial(p);
      return a;
    }
    static std::unique_ptr<FiniteAutomaton> Union(std::unique_ptr<FiniteAutomaton> a,
                                                  std::unique_ptr<FiniteAutomaton> b) {
      auto p = std::make_shared<FiniteAutomatonState>();
      p->AddEpsilon(a->initial_state);
      p->AddEpsilon(b->initial_state);
      a->SetDeterministic(false);
      a->SetInitial(p);
      return a;
    }
    static std::unique_ptr<FiniteAutomaton> Concat(std::unique_ptr<FiniteAutomaton> a,
                                                   std::unique_ptr<FiniteAutomaton> b) {
      auto ac = a->GetAcceptStates();

      for (const auto& p : ac) {
        p->SetAccept(false);
        p->AddEpsilon(b->initial_state);
      }
      a->SetDeterministic(a->deterministic && b->deterministic);
      return a;
    }
    static std::unique_ptr<FiniteAutomaton> CharRangeAutomaton(uint32_t min, uint32_t max) {
      auto p = std::make_shared<FiniteAutomatonState>();
      auto q = std::make_shared<FiniteAutomatonState>();
      auto fa = std::make_unique<FiniteAutomaton>(p);

      q->SetAccept();
      p->AddTransition(min, max, q);
      fa->SetDeterministic(true);

      return fa;
    }
    static std::unique_ptr<FiniteAutomaton> StringAutomaton(const std::string& str) {
      // maybe rename it works with only chars utf8 included now
      auto b1 = str[0];
      auto len = ZRegex::Utf8::multiByteSequenceLength(b1);
      auto c = ZRegex::Utf8::ReadMultiByteCase(str, b1, len);
      auto p = std::make_shared<FiniteAutomatonState>();
      auto fa = std::make_unique<FiniteAutomaton>(p);

      auto q = std::make_shared<FiniteAutomatonState>();
      p->AddTransition(c, c, q);

      q->SetAccept();
      fa->SetDeterministic(true);
      return fa;
    }

    static std::unique_ptr<FiniteAutomaton> Intersect(std::unique_ptr<FiniteAutomaton> a,
                                                      std::unique_ptr<FiniteAutomaton> b) {
      //      auto fa = std::make_unique<FiniteAutomaton>();
      //      auto t1 = a->GetT
    }

    // Named Char classes
    static std::unique_ptr<FiniteAutomaton> AlphaNumCharClass() {
      return FAFactory::Union(ZRegex::FAFactory::AlphaCharClass(),
                              ZRegex::FAFactory::DigitCharClass());
    }
    static std::unique_ptr<FiniteAutomaton> AlphaCharClass() {
      return FAFactory::Union(ZRegex::FAFactory::UpperCharClass(),
                              ZRegex::FAFactory::LowerCharClass());
    }

    static std::unique_ptr<FiniteAutomaton> UpperCharClass() {
      return ZRegex::FAFactory::CharRangeAutomaton(65, 90);
    }

    static std::unique_ptr<FiniteAutomaton> LowerCharClass() {
      return ZRegex::FAFactory::CharRangeAutomaton(97, 122);
    }

    static std::unique_ptr<FiniteAutomaton> DigitCharClass() {
      return ZRegex::FAFactory::CharRangeAutomaton(48, 57);
    }
  };
}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_FACTORY_H
