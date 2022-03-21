#include "bench.h"
#define DS_TPCHORDERS "/home/elzarei/tpch-data/out/orders.tbl.csv.txt"
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=special.*packages", u8"special.*packages", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=special%packages", u8"special%packages", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32);

