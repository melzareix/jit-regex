#include "bench.h"
#define DS_TPCHLINEITEM "/home/elzarei/tpch-data/out/lineitem.tbl.csv.txt"
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ular", u8"ular", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHLINEITEM);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ular", u8"ular", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHLINEITEM)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=ular", u8"ular", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHLINEITEM);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=ular", u8"ular", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHLINEITEM)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=ular", u8"ular", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHLINEITEM);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=ular", u8"ular", ZRegex::CodegenOpts() /*encoding*/, DS_TPCHLINEITEM);

