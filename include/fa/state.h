//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_STATE_H
#define ZREGEXSTANDALONE_STATE_H

#include <algorithm>
#include <cstdint>
#include <memory>
#include <unordered_set>

#include "fa/transition.h"
#include "spdlog/spdlog.h"

namespace ZRegex {
  class FiniteAutomatonState {
  public:
    inline static uint32_t id_counter = 0;  // global counter for created states ids

    uint32_t id;
    bool accept;
    uint8_t chr = '\0';
    std::unordered_set<ZRegex::FiniteAutomatonTransition,
                       ZRegex::FiniteAutomatonTransition::HashFunction>
        transitions;

    FiniteAutomatonState() : accept(false), id(id_counter++){};
    // ~FiniteAutomatonState() { spdlog::debug("FiniteAutomatonState {} Destructed!", id); }

    void AddTransition(const uint32_t& min, const uint32_t& max,
                       const std::shared_ptr<FiniteAutomatonState>& to) {
      transitions.emplace(min, max, to);
    }
    void AddEpsilon(const std::shared_ptr<FiniteAutomatonState>& to) {
      if (to->accept) {
        this->SetAccept(true);
      }
      transitions.insert(to->transitions.begin(), to->transitions.end());
    }
    void SetAccept(bool acc = true) { accept = acc; }
    static void ResetCounter() { id_counter = 0; }
    void ResetTransitions() { transitions.clear(); }
    std::vector<FiniteAutomatonTransition> GetSortedTransitions() {
      std::vector<FiniteAutomatonTransition> results;
      results.insert(results.end(), transitions.begin(), transitions.end());
      std::sort(results.begin(), results.end());
      return results;
    }
    friend bool operator==(const FiniteAutomatonState& s1, const FiniteAutomatonState& s2);
    friend bool operator<(const FiniteAutomatonState& s1, const FiniteAutomatonState& s2);

    [[nodiscard]] size_t Hash() const { return std::hash<uint32_t>()(id); }

    struct HashFunction {
      size_t operator()(const FiniteAutomatonState& s) const {
        size_t id_hash = std::hash<uint32_t>()(s.id);
        return id_hash;
      }
    };
  };
}  // namespace ZRegex
#endif  // ZREGEXSTANDALONE_STATE_H
