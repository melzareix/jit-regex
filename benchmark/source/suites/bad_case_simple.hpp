#include "bench.h"
#define DS_BAD_CASE_SIMPLE "/home/elzarei/bench_data/bad_case.txt"
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=ßääääääääääääääääääääääääääääääääääääääääääääääääää", u8"ßääääääääääääääääääääääääääääääääääääääääääääääääää", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE_SIMPLE)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ßääääääääääääääääääääääääääääääääääääääääääääääääää", u8"ßääääääääääääääääääääääääääääääääääääääääääääääääää", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE_SIMPLE)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ßääääääääääääääääääääääääääääääääääääääääääääääääää", u8"ßääääääääääääääääääääääääääääääääääääääääääääääääää", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE_SIMPLE)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=ßääääääääääääääääääääääääääääääääääääääääääääääääää", u8"ßääääääääääääääääääääääääääääääääääääääääääääääääää", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE_SIMPLE)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=ßääääääääääääääääääääääääääääääääääääääääääääääääää", u8"ßääääääääääääääääääääääääääääääääääääääääääääääääää", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE_SIMPLE)->Unit(benchmark::kMicrosecond);

