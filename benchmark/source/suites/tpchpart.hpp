#include "bench.h"
#define DS_TPCHPART "/home/elzarei/tpch-data/out/part.tbl.csv.txt"
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=epo", u8"epo", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHPART)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=epo", u8"epo", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHPART)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=epo", u8"epo", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHPART)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=epo", u8"epo", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHPART)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=epo", u8"epo", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHPART)->Unit(benchmark::kMicrosecond);

