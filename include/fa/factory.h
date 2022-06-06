//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_FACTORY_H
#define ZREGEXSTANDALONE_FACTORY_H

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "fa/fa.h"
#include "helpers/utf8.h"
#include "helpers/utf8range.h"

typedef std::vector<std::vector<uint8_t>> utf8_range;
typedef std::tuple<std::shared_ptr<ZRegex::FiniteAutomatonState>,
                   std::shared_ptr<ZRegex::FiniteAutomatonState>,
                   std::shared_ptr<ZRegex::FiniteAutomatonState>>
    state_tuple;
typedef std::tuple<std::shared_ptr<ZRegex::FiniteAutomatonState>,
                   std::shared_ptr<ZRegex::FiniteAutomatonState>>
    state_pair;

namespace ZRegex {
  struct FAFactory {
    static std::unique_ptr<FiniteAutomaton> RangeDFA(std::vector<utf8_range> ranges) {
      std::vector<std::unique_ptr<FiniteAutomaton>> dfas;
      for (auto range : ranges) {
        std::vector<std::unique_ptr<FiniteAutomaton>> range_dfa;
        for (auto element : range) {
          // guranteed to have two elements
          range_dfa.push_back(CharRangeAutomaton(element[0], element[1]));
        }
        dfas.push_back(ConcatAll(range_dfa));
      }
      return std::move(UnionAll(dfas));
    }

    static std::unique_ptr<FiniteAutomaton> AnyCharByteDFA() {
      auto from_u8 = std::string(u8"\u0000");
      auto to_u8 = std::string(u8"􏿿");
      auto range_ = UTF8Range(from_u8, to_u8);
      std::vector<std::vector<std::vector<uint8_t>>> ranges;
      while (range_.hasNext()) {
        ranges.push_back(range_.NextRange());
        // spdlog::warn("{} - {}", ranges.back()[0][0], ranges.back()[0][1]);
      }
      return std::move(FAFactory::RangeDFA(ranges));
    }
    static std::unique_ptr<FiniteAutomaton> AnyChar(bool byte_dfa_utf8_ = false) {
      if (byte_dfa_utf8_) {
        return AnyCharByteDFA();
      }
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
    static std::unique_ptr<FiniteAutomaton> ByteAutomaton(const std::string& str) {
      auto p = std::make_shared<FiniteAutomatonState>();
      auto fa = std::make_unique<FiniteAutomaton>(p);
      for (unsigned char c : str) {
        auto q = std::make_shared<FiniteAutomatonState>();
        p->AddTransition(c, c, q);
        p = q;
        p->chr = c;
      }
      p->SetAccept();
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
      q->chr = b1;
      q->SetAccept();
      fa->SetDeterministic(true);
      return fa;
    }

    static std::unique_ptr<FiniteAutomaton> Intersect(std::unique_ptr<FiniteAutomaton> a,
                                                      std::unique_ptr<FiniteAutomaton> b) {
      auto transitions1 = a->GetSortedTransitionsVector();
      auto transitions2 = b->GetSortedTransitionsVector();

      auto is = std::make_shared<FiniteAutomatonState>();
      auto fa = std::make_unique<FiniteAutomaton>(is);
      std::map<state_pair, std::shared_ptr<ZRegex::FiniteAutomatonState>> new_states;
      std::list<state_tuple> worklist;
      auto sp = std::make_tuple(is, a->initial_state, b->initial_state);

      worklist.push_back(sp);
      auto abtuple = std::make_tuple(a->initial_state, b->initial_state);
      new_states[abtuple] = is;

      while (!worklist.empty()) {
        auto p = worklist.front();
        worklist.pop_front();

        // accept if both accept
        std::get<0>(p)->SetAccept(std::get<1>(p)->accept && std::get<2>(p)->accept);

        auto t1 = transitions1[std::get<1>(p)->id];
        auto t2 = transitions2[std::get<2>(p)->id];

        for (uint32_t n1 = 0, b2 = 0; n1 < t1.size(); n1++) {
          while (b2 < t2.size() && t2[b2].max < t1[n1].min) b2++;
          for (int n2 = b2; n2 < t2.size() && t1[n1].max >= t2[n2].min; n2++) {
            if (t2[n2].max >= t1[n1].min) {
              auto q = std::make_tuple(t1[n1].to, t2[n2].to);
              auto notfound = new_states.find(q) == new_states.end();
              auto ns = std::make_shared<FiniteAutomatonState>();
              if (notfound) {
                worklist.push_back(std::make_tuple(ns, t1[n1].to, t2[n2].to));
                new_states[q] = ns;
              } else {
                ns = new_states[q];
              }
              uint32_t min = t1[n1].min > t2[n2].min ? t1[n1].min : t2[n2].min;
              uint32_t max = t1[n1].max < t2[n2].max ? t1[n1].max : t2[n2].max;
              std::get<0>(p)->AddTransition(min, max, ns);
            }
          }
        }
      }
      fa->SetDeterministic(a->deterministic && b->deterministic);
      fa->RemoveDeadStates();

      return fa;
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
