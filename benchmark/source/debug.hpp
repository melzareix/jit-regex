#include "suites/bench.h"
#pragma once

// debug benchmarks results
// void cmp(char* pattern, char* dataset) {
//   std::ifstream st(dataset, std::ios_base::binary);
//   std::string line;
//   double matches = 0;
//   auto p = re2::RE2("special.*packages");

//   auto patterns = SplitString(pattern, "%");
//   auto epsm1 = ZRegex::EPSMMatcher(patterns[0].c_str(), patterns[0].size());
//   auto epsm2 = ZRegex::EPSMMatcher(patterns[1].c_str(), patterns[1].size());

//   auto b1 = epsm1.preprocess();
//   auto b2 = epsm2.preprocess();
//   bool x, y, z;
//   int cnt = 0;
//   while (getline(st, line)) {
//     // auto p1 = epsm1.epsm2_search_find_first(line.c_str(), line.size());
//     // auto p2 = epsm2.epsm2_search_find_last(line.c_str(), line.size());
//     auto p3 = epsm1.epsma_elz_early(line.c_str(), line.size(), b1);
//     auto p4 = epsm2.epsma_elz(line.c_str(), line.size(), b2);
//     x = re2::RE2::PartialMatch(line, p);
//     // y = p1 != -1 && p2 != -1 && p2 >= p1;
//     z = p3 != -1 && p4 != -1 && p3 <= p4;
//     // spdlog::warn("Result: {} - {} - {}", x, p3, p4);
//     if (x != z) {
//       spdlog::error(line);
//       // spdlog::warn("{} - {}", p1, p2);
//       spdlog::error("Fail: {} - {} - {}", x, p3, p4);
//       auto p5 = epsm1.epsma_elz_early(line.c_str(), line.size(), b1);
//       auto p6 = epsm2.epsma_elz(line.c_str(), line.size(), b2);

//       return;
//     }
//   }
// }

void test() {
  using namespace ZRegex;
  auto opts = CodegenOpts();
  // opts.SetByteDFA(true);
  ZRegex::Codegen code_generator(opts);
  auto dfa = RegExp::GetAutomatonForPattern("[^abcdef]", opts.IsByteDFA());

  // auto dfa2 = RegExp::GetAutomatonForPattern("a", opts.IsByteDFA());
  // dfa2->Complement(true);
  // dfa2->Visualize();

  // auto dfa3 = FAFactory::Intersect(std::move(dfa), std::move(dfa2));
  // auto d3 = dfa3.get();
  // dfa->Visualize();
}
void cmp2(char* pattern1, char* pattern2, char* dataset) {
  using namespace ZRegex;
  std::ifstream st(dataset, std::ios_base::binary);
  std::string line;
  double matches = 0;
  auto p = re2::RE2(pattern1);

  auto opts = CodegenOpts();
  ZRegex::Codegen code_generator(opts);
  code_generator.Compile(pattern2);

  bool x, y, z;
  int cnt = 0;
  while (getline(st, line)) {
    // auto p1 = epsm1.epsm2_search_find_first(line.c_str(), line.size());
    // auto p2 = epsm2.epsm2_search_find_last(line.c_str(), line.size());
    x = re2::RE2::PartialMatch(line, p);
    // y = p1 != -1 && p2 != -1 && p2 >= p1;
    z = code_generator.Run(line.c_str());
    // spdlog::warn("Result: {} - {} - {}", x, p3, p4);
    if (x != z) {
      spdlog::error(line);
      spdlog::warn("{} - {}", x, z);

      return;
    }
  }
}
