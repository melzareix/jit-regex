#include "bench.h"
#define DS_TPCHORDERS "/home/elzarei/tpch-data/out/orders.tbl.csv.txt"
BENCHMARK_CAPTURE(BENCH_RE2, "p=special.*packages", u8"special.*packages",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_PCRE2, "p=special.*packages", u8"special.*packages",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_BOOST, "p=special.*packages", u8"special.*packages",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BENCH_SIMD_MULTIPATTERN, "p=special.*packages", u8"special%packages",
                  ZRegex::CodegenOpts() /*encoding*/, DS_TPCHORDERS)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);
