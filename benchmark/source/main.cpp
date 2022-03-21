#include "suites/tpchlineitem.hpp"
#include "suites/tpchorders.hpp"
#include "suites/bad_case_simple.hpp"
#include "suites/tpchpart.hpp"
#include "suites/regex_dna.hpp"
#include "suites/bad_case.hpp"

    int main(int argc, char** argv) {
      ::benchmark::Initialize(&argc, argv);
      spdlog::cfg::load_argv_levels(argc, argv);
      ::benchmark::RunSpecifiedBenchmarks();
      ::benchmark::Shutdown();
      return 0;
    }
    
