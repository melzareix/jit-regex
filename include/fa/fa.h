//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_NFA_H
#define ZREGEXSTANDALONE_NFA_H

#include <helpers/utf8.h>

#include <cstdint>
#include <initializer_list>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>

#include "fa/state.h"

namespace ZRegex {
  typedef std::unordered_set<std::shared_ptr<FiniteAutomatonState>> fa_st;
  class FiniteAutomaton {
  public:
    bool deterministic;
    std::shared_ptr<FiniteAutomatonState> initial_state;

    // Destructor
    ~FiniteAutomaton() { spdlog::debug("FA Destructed!"); }

    // Constructor
    explicit FiniteAutomaton(std::shared_ptr<FiniteAutomatonState> initial)
        : deterministic(false), initial_state(initial){};

    // Copy Constructor
    FiniteAutomaton(const FiniteAutomaton& a);

    // Setters
    void SetInitial(std::shared_ptr<FiniteAutomatonState> initial);
    void SetDeterministic(bool det = true);

    // Getters
    [[nodiscard]] fa_st GetStates() const;
    [[nodiscard]] fa_st GetAcceptStates() const;
    [[nodiscard]] std::vector<uint32_t> GetStartPoints() const;

    // Operations
    void Complement();
    fa_st GetLiveStates() const;
    void RemoveDeadStates();
    void Totalize();
    void Determinize();
    void Visualize() const;
    void Reduce();
    // Hashing
    struct Hasher {
      size_t operator()(const fa_st& V) const {
        auto hash = V.size();
        for (auto& i : V) {
          hash ^= i->Hash();
        }
        return hash;
      }
    };

    // Hashing
    struct StateHasher {
      size_t operator()(const FiniteAutomatonState& V) const { return V.Hash(); }
    };
  };
}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_NFA_H
