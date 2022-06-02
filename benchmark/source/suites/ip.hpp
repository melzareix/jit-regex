#include "bench.h"
#define DS_IP "/home/elzarei/reddit_data/dataset.txt"
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=[0-9]+.[0-9]+.[0-9]+.[0-9]+", "fuck",
                  ZRegex::CodegenOpts() /*encoding*/, DS_IP)
    ->Unit(benchmark::kSecond);

BENCHMARK_CAPTURE(BENCH_SIMD_EPSM, u8"p=[0-9]+.[0-9]+.[0-9]+.[0-9]+", "fuck",
                  ZRegex::CodegenOpts() /*encoding*/, DS_IP)
    ->Unit(benchmark::kSecond);

BENCHMARK_CAPTURE(BENCH_PCRE2, u8"p=[0-9]+.[0-9]+.[0-9]+.[0-9]+", "fuck",
                  ZRegex::CodegenOpts() /*encoding*/, DS_IP)
    ->Unit(benchmark::kSecond);

// BENCHMARK_CAPTURE(BENCH_PCRE2_DFA, u8"p=[0-9]+.[0-9]+.[0-9]+.[0-9]+",
//                   "[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+",
//                   ZRegex::CodegenOpts() /*encoding*/, DS_IP)
//     ->Unit(benchmark::kSecond);

BENCHMARK_CAPTURE(BENCH_DFA, u8"p=[0-9]+.[0-9]+.[0-9]+.[0-9]+", "fuck",
                  ZRegex::CodegenOpts() /*encoding*/, DS_IP)
    ->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)
    ->Unit(benchmark::kSecond);

// BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=ular", "[a-zA-Z0-9_+]", ZRegex::CodegenOpts() /*encoding*/,
//                   DS_IP)
//     ->Unit(benchmark::kSecond);
