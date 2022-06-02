#include "../bench.h"
// #include "../benchcmp.hpp"

#define DS_TPCHORDERS "/home/elzarei/tpch-data/out/orders.tbl.csv.txt"

BENCHMARK_CAPTURE(BENCH_SIMD_MULTIPATTERN, u8"p=special%packages", u8"special%packages",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS);
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=special.*packages", u8"special.*packages",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS);
BENCHMARK_CAPTURE(BENCH_PCRE2, u8"p=special.*packages", u8".*special.*packages.*",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=special%packages", u8"%special%packages%",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS)
    ->DenseRange(DFA_LLVM_U8, DFA_CPP_U32);

// BENCHMARK_CAPTURE(BENCH_DFA, "sql", ZRegex::CodegenOpts() /*encoding*/,
// "/home/elzarei/jit-regex/benchmark/scripts/../patterns/sql.txt")->DenseRange(DFA_LLVM_U8,
// DFA_CPP_U32)->Unit(benchmark::kSecond)->UseRealTime();
