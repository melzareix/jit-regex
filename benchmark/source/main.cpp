#include "suites/bad_case.hpp"
#include "suites/bad_case_simple.hpp"
#include "suites/regex_dna.hpp"
#include "suites/tpchlineitem.hpp"
#include "suites/tpchorders.hpp"
#include "suites/tpchpart.hpp"

void count_single() {
  std::string line
      = "\"re carefully since the slyly unusual instructions. final, special packages are\"";
  std::string pattern = "special packages";
  auto barr = ZRegex::SIMDSubstringMatch::preprocess(pattern.c_str(), pattern.size());
  auto pcstr = pattern.c_str();
  auto linecstr = line.c_str();
  auto m = pattern.size();

  auto r1 = ZRegex::SIMDSubstringMatch::epsm_a(pcstr, m, linecstr, line.size(), barr);
  spdlog::info("{}", r1);
}

void count() {
  uint64_t cnt = 0;
  uint64_t recount = 0;
  std::string line;
  std::string pattern = "packages";
  std::ifstream st(DS_TPCHORDERS, std::ios_base::binary);
  auto barr = ZRegex::SIMDSubstringMatch::preprocess(pattern.c_str(), pattern.size());
  auto pcstr = pattern.c_str();
  auto m = pattern.size();

  re2::RE2 p = re2::RE2(pattern);
  auto diff = 0;
  while (getline(st, line)) {
    // auto r1 = ZRegex::SIMDSubstringMatch::epsm_a(pcstr, m, line.c_str(), line.size(), barr);
    auto r1 = ZRegex::SIMDSubstringMatch::sse4_strstr_anysize(line.c_str(), line.size(), pcstr, m);
    cnt += r1;
    auto r2 = re2::RE2::PartialMatch(line, p);
    recount += r2;
    if (r1 != r2) {
      diff++;
      spdlog::warn("Wrong SIMD: re2 {} - simd {} - \"{}\"", r2, r1, line);
      // auto q = ZRegex::SIMDSubstringMatch::epsm_a(pcstr, m, line.c_str(), line.size(), barr);
      auto q = ZRegex::SIMDSubstringMatch::sse4_strstr_anysize(line.c_str(), line.size(), pcstr, m);
      spdlog::warn("{}", q);
    }
  }

  spdlog::warn("recount: {}", recount);
  spdlog::warn("diff: {}", diff);
  spdlog::warn("count: {}", cnt);
}

int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  spdlog::cfg::load_argv_levels(argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  // count();
  // count_single();
  return 0;
}
