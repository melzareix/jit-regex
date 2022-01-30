//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_NFA_H
#define ZREGEXSTANDALONE_NFA_H

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>

#include "fa/state.h"
#include "parser/grammar/regexLexer.h"
#include "parser/grammar/regexParser.h"

namespace ZRegex {
  class FiniteAutomaton {
  public:
    bool deterministic;
    std::shared_ptr<FiniteAutomatonState> initial_state;

    ~FiniteAutomaton() { spdlog::debug("FA Destructed!"); }
    explicit FiniteAutomaton(std::shared_ptr<FiniteAutomatonState> initial)
        : deterministic(false), initial_state(initial){};

    void SetInitial(std::shared_ptr<FiniteAutomatonState> initial) { initial_state = initial; }
    void SetDeterministic(bool det = true) { deterministic = det; }
    [[nodiscard]] auto GetStates() const {
      std::unordered_set<std::shared_ptr<FiniteAutomatonState>> visited;
      std::list<std::shared_ptr<FiniteAutomatonState>> worklist;

      worklist.push_back(initial_state);
      visited.insert(initial_state);

      while (!worklist.empty()) {
        auto s = worklist.front();
        worklist.pop_front();
        for (const auto& t : s->transitions) {
          if (visited.find(t.to) == visited.end()) {
            visited.insert(t.to);
            worklist.push_back(t.to);
          }
        }
      }

      return visited;
    }
    void Visualize() const {
      std::ofstream fs("/tmp/regex.dot");
      fs << "digraph Automaton {" << std::endl;
      fs << "rankdir = LR;" << std::endl;
      auto states = GetStates();
      for (const auto& s : states) {
        fs << " " << s->id << " ";
        if (s->accept) {
          fs << "[shape=doublecircle,label=\"" << s->id << R"(",style=filled,fillcolor="green"];)"
             << std::endl;
        } else {
          fs << "[shape=circle,label=\"" << s->id << "\"];" << std::endl;
        }
        if (s->id == initial_state->id) {
          fs << "initial [shape=plaintext,label=\"\"];" << std::endl;
          fs << " initial -> " << s->id << std::endl;
        }

        for (const auto& t : s->transitions) {
          fs << " " << s->id;
          fs << " -> " << t.to->id << "[label=\"";
          if (t.min != t.max) {
            fs << t.min << "-" << t.max;
          } else {
            fs << t.min;
          }
          fs << "\"]" << std::endl;
        }
      }
      fs << "}" << std::endl;
      spdlog::info("Dot Visualization of Regex");
      system("cat /tmp/regex.dot | graph-easy");
    }
    [[nodiscard]] auto GetAcceptStates() const {
      std::unordered_set<std::shared_ptr<FiniteAutomatonState>> accepts;

      std::unordered_set<std::shared_ptr<FiniteAutomatonState>> visited;
      std::list<std::shared_ptr<FiniteAutomatonState>> worklist;

      worklist.push_back(initial_state);
      visited.insert(initial_state);

      while (!worklist.empty()) {
        auto s = worklist.front();
        worklist.pop_front();
        if (s->accept) {
          accepts.insert(s);
        }

        for (const auto& t : s->transitions) {
          if (visited.find(t.to) == visited.end()) {
            visited.insert(t.to);
            worklist.push_back(t.to);
          }
        }
      }

      return accepts;
    }

    std::vector<uint16_t> GetStartPoints() {
      std::unordered_set<uint16_t> point_set;
      std::vector<uint16_t> points;
      point_set.insert(0);
      auto states = GetStates();
      for (const auto& s : states) {
        for (const auto& t : s->transitions) {
          point_set.insert(t.min);
          if (t.max < 127) {
            point_set.insert(t.max + 1);
          }
        }
      }

      points.reserve(point_set.size());
      uint32_t i = 0;
      for (unsigned short it : point_set) {
        points.push_back(it);
      }
      std::sort(points.begin(), points.end());
      return points;
    }

    typedef std::unordered_set<std::shared_ptr<FiniteAutomatonState>> fa_st;
    struct Hasher {
      size_t operator()(const fa_st& V) const {
        auto hash = V.size();
        for (auto& i : V) {
          hash ^= i->Hash();
        }
        return hash;
      }
    };
    void Complement() {
      this->Determinize();
      this->Totalize();
      for (const auto& s: this->GetStates()) {
        s->SetAccept(false);
      }
      this->RemoveDeadStates();
    }
    void RemoveDeadStates(){}
    void Totalize() {
    }
    void Determinize() {
      auto points = GetStartPoints();
      std::list<fa_st> worklist;
      std::unordered_map<fa_st, std::shared_ptr<FiniteAutomatonState>, Hasher> new_state;
      fa_st initial_set = {initial_state};
      worklist.push_back(initial_set);

      initial_state = std::make_shared<FiniteAutomatonState>();
      new_state[initial_set] = initial_state;

      auto iter = 0;
      while (!worklist.empty()) {
        // probably will fail fallback
        if (iter > 1000) {
          throw std::runtime_error("Pattern too large! Potential Exponential State Explosion!");
        }
        auto s = worklist.front();
        worklist.pop_front();
        auto r = new_state[s];
        //        auto r = std::make_shared<FiniteAutomatonState>();

        // new superset is accepting if one of the substates is accepting
        for (const auto& q : s) {
          if (q->accept) {
            r->SetAccept(true);
            break;
          }
        }

        for (size_t i = 0; i < points.size(); ++i) {
          fa_st p;
          for (const auto& q : s) {
            for (const auto& t : q->transitions) {
              if (t.min <= points[i] && points[i] <= t.max) {
                p.insert(t.to);
              }
            }
          }

          std::shared_ptr<FiniteAutomatonState> q;
          if (!p.empty()) {
            if (new_state.count(p) == 0) {
              worklist.push_back(p);
              q = new_state[p] = std::make_shared<FiniteAutomatonState>();
            } else {
              q = new_state[p];
            }

            uint16_t min = points[i];
            uint16_t max;

            if (i + 1 < points.size()) {
              max = points[i + 1] - 1;
            } else {
              max = 127;
            }

            r->AddTransition(min, max, q);
          }
        }
        iter++;
      }
      this->SetDeterministic(true);
    }

    FiniteAutomaton(const FiniteAutomaton& a) {
      spdlog::debug("FA Copy Constructor");
      deterministic = a.deterministic;
      std::unordered_map<uint32_t, std::shared_ptr<FiniteAutomatonState>> map;
      auto states = a.GetStates();
      for (const auto& s : states) {
        map[s->id] = std::make_shared<FiniteAutomatonState>();
      }

      for (const auto& s : states) {
        auto p = map[s->id];
        p->SetAccept(s->accept);
        if (s->id == a.initial_state->id) {
          SetInitial(p);
        }
        for (const auto& t : s->transitions) {
          p->AddTransition(t.min, t.max, map[t.to->id]);
        }
      }
    }
  };
}  // namespace ZRegex

#endif  // ZREGEXSTANDALONE_NFA_H
