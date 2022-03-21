#include "suites/bad_case.hpp"
#include "suites/bad_case_simple.hpp"
#include "suites/regex_dna.hpp"
#include "suites/tpch.hpp"
#include "suites/tpchpart.hpp"

int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  spdlog::cfg::load_argv_levels(argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}
