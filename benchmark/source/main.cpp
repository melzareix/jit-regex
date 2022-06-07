// #include "suites/compile/common.hpp"
// #include "suites/compile/sql.hpp"
#include "suites/regex_dna.hpp"
// #include "suites/spark_logs.hpp"

int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  spdlog::cfg::load_argv_levels(argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}
