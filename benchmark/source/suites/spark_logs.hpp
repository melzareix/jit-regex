#include "bench.h"
#define DS_SPARK_LOGS "/home/elzarei/spark/logs.log"
BENCHMARK_CAPTURE(BENCH_RE2, "p=0", "([a-zA-Z][a-zA-Z0-9]*)://([^/]+)(/[^ ]*)?", ZRegex::CodegenOpts() /*encoding*/, DS_SPARK_LOGS)->Unit(benchmark::kSecond);
BENCHMARK_CAPTURE(BENCH_PCRE2, "p=0", "([a-zA-Z][a-zA-Z0-9]*)://([^/]+)(/[^ ]*)?", ZRegex::CodegenOpts() /*encoding*/, DS_SPARK_LOGS)->Unit(benchmark::kSecond);
BENCHMARK_CAPTURE(BENCH_DFA, "p=0", "([a-zA-Z][a-zA-Z0-9]*)://([^/]+)(/[^ ]*)?", ZRegex::CodegenOpts() /*encoding*/, DS_SPARK_LOGS)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kSecond);

BENCHMARK_CAPTURE(BENCH_RE2, "p=1", "([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?|[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+[.][a-zA-Z0-9-.]+", ZRegex::CodegenOpts() /*encoding*/, DS_SPARK_LOGS)->Unit(benchmark::kSecond);
BENCHMARK_CAPTURE(BENCH_PCRE2, "p=1", "([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?|[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+[.][a-zA-Z0-9-.]+", ZRegex::CodegenOpts() /*encoding*/, DS_SPARK_LOGS)->Unit(benchmark::kSecond);
BENCHMARK_CAPTURE(BENCH_DFA, "p=1", "([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?|[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+[.][a-zA-Z0-9-.]+", ZRegex::CodegenOpts() /*encoding*/, DS_SPARK_LOGS)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kSecond);

