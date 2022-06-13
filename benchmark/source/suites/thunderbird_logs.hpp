#include "bench.h"
#define DS_THUNDERBIRD_LOGS "/home/elzarei/thunderbird/Thunderbird.log"
BENCHMARK_CAPTURE(BENCH_RE2, "p=(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", "(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", ZRegex::CodegenOpts() /*encoding*/, DS_THUNDERBIRD_LOGS)->Unit(benchmark::kSecond);
BENCHMARK_CAPTURE(BENCH_PCRE2, "p=(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", "(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", ZRegex::CodegenOpts() /*encoding*/, DS_THUNDERBIRD_LOGS)->Unit(benchmark::kSecond);
BENCHMARK_CAPTURE(BENCH_DFA, "p=(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", "(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", ZRegex::CodegenOpts() /*encoding*/, DS_THUNDERBIRD_LOGS)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kSecond);
BENCHMARK_CAPTURE(BENCH_BOOST, "p=(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", "(0[1-9]|[12][0-9]|3[01])[.](0[1-9]|1[012])[.](19|20)[0-9]{2}", ZRegex::CodegenOpts() /*encoding*/, DS_THUNDERBIRD_LOGS)->Unit(benchmark::kSecond);

