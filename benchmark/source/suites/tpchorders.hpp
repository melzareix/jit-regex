#include "bench.h"
#define DS_TPCHORDERS "/home/elzarei/tpch-data/out/orders.tbl.csv.txt"
// BENCHMARK_CAPTURE(BENCH_SIMD_MULTIPATTERN, u8"p=pack%pack", u8"pack%pack",
//                   ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=pack", u8"pack",
                  ZRegex::CodegenOpts()
                  /*encoding*/,
                  DS_TPCHORDERS);
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=pack", u8"pack", ZRegex::CodegenOpts() /*encoding*/,
                  DS_TPCHORDERS);
// BENCHMARK_CAPTURE(BENCH_DFA, u8"p=pack", u8"pack", ZRegex::CodegenOpts() /*encoding*/,
//                   DS_TPCHORDERS)
//     ->DenseRange(DFA_LLVM_U8, DFA_LLVM_U8);
BENCHMARK_CAPTURE(BENCH_SIMD_EPSM, u8"p=pack", u8"pack", ZRegex::CodegenOpts() /*encoding*/,
                  DS_TPCHORDERS);
