#include "bench.h"
#define DS_BAD_CASE "/home/elzarei/bench_data/bad_case.txt"
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ä", u8"ä", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ä", u8"ä", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ä{1,5}", u8"ä{1,5}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ä{1,5}", u8"ä{1,5}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ä{50,55}", u8"ä{50,55}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ä{50,55}", u8"ä{50,55}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ä{56,}", u8"ä{56,}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ä{56,}", u8"ä{56,}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ßä{50}", u8"ßä{50}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ßä{50}", u8"ßä{50}", ZRegex::CodegenOpts() /*encoding*/, DS_BAD_CASE)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMillisecond);

