#include "../benchcmp.hpp"
BENCHMARK_CAPTURE(BENCH_DFA, "common", ZRegex::CodegenOpts() /*encoding*/, "/home/elzarei/jit-regex/benchmark/scripts/../patterns/common.txt")->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kSecond)->UseRealTime();

