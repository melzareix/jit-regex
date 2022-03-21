#include "bench.h"
#define DS_REGEX_DNA "/home/elzarei/jit-regex/benchmark/data/regexdna.txt"
BENCHMARK_CAPTURE(BENCH_RE2, u8"p=agggtaaa|tttaccct", u8"agggtaaa|tttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=agggtaaa|tttaccct", u8"agggtaaa|tttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=agggtaaa|tttaccct", u8"agggtaaa|tttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=agggtaaa|tttaccct", u8"agggtaaa|tttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=agggtaaa|tttaccct", u8"agggtaaa|tttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=agggtaaa|tttaccct", u8"agggtaaa|tttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=[cgt]gggtaaa|tttaccc[acg]", u8"[cgt]gggtaaa|tttaccc[acg]", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=[cgt]gggtaaa|tttaccc[acg]", u8"[cgt]gggtaaa|tttaccc[acg]", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=[cgt]gggtaaa|tttaccc[acg]", u8"[cgt]gggtaaa|tttaccc[acg]", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=[cgt]gggtaaa|tttaccc[acg]", u8"[cgt]gggtaaa|tttaccc[acg]", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=[cgt]gggtaaa|tttaccc[acg]", u8"[cgt]gggtaaa|tttaccc[acg]", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=[cgt]gggtaaa|tttaccc[acg]", u8"[cgt]gggtaaa|tttaccc[acg]", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=a[act]ggtaaa|tttacc[agt]t", u8"a[act]ggtaaa|tttacc[agt]t", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=a[act]ggtaaa|tttacc[agt]t", u8"a[act]ggtaaa|tttacc[agt]t", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=a[act]ggtaaa|tttacc[agt]t", u8"a[act]ggtaaa|tttacc[agt]t", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=a[act]ggtaaa|tttacc[agt]t", u8"a[act]ggtaaa|tttacc[agt]t", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=a[act]ggtaaa|tttacc[agt]t", u8"a[act]ggtaaa|tttacc[agt]t", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=a[act]ggtaaa|tttacc[agt]t", u8"a[act]ggtaaa|tttacc[agt]t", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=agg[act]taaa|ttta[agt]cct", u8"agg[act]taaa|ttta[agt]cct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=agg[act]taaa|ttta[agt]cct", u8"agg[act]taaa|ttta[agt]cct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=agg[act]taaa|ttta[agt]cct", u8"agg[act]taaa|ttta[agt]cct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=agg[act]taaa|ttta[agt]cct", u8"agg[act]taaa|ttta[agt]cct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=agg[act]taaa|ttta[agt]cct", u8"agg[act]taaa|ttta[agt]cct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=agg[act]taaa|ttta[agt]cct", u8"agg[act]taaa|ttta[agt]cct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=aggg[acg]aaa|ttt[cgt]ccct", u8"aggg[acg]aaa|ttt[cgt]ccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=aggg[acg]aaa|ttt[cgt]ccct", u8"aggg[acg]aaa|ttt[cgt]ccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=aggg[acg]aaa|ttt[cgt]ccct", u8"aggg[acg]aaa|ttt[cgt]ccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=aggg[acg]aaa|ttt[cgt]ccct", u8"aggg[acg]aaa|ttt[cgt]ccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=aggg[acg]aaa|ttt[cgt]ccct", u8"aggg[acg]aaa|ttt[cgt]ccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=aggg[acg]aaa|ttt[cgt]ccct", u8"aggg[acg]aaa|ttt[cgt]ccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=agggtaa[cgt]|[acg]ttaccct", u8"agggtaa[cgt]|[acg]ttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=agggtaa[cgt]|[acg]ttaccct", u8"agggtaa[cgt]|[acg]ttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=agggtaa[cgt]|[acg]ttaccct", u8"agggtaa[cgt]|[acg]ttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=agggtaa[cgt]|[acg]ttaccct", u8"agggtaa[cgt]|[acg]ttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=agggtaa[cgt]|[acg]ttaccct", u8"agggtaa[cgt]|[acg]ttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=agggtaa[cgt]|[acg]ttaccct", u8"agggtaa[cgt]|[acg]ttaccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=agggta[cgt]a|t[acg]taccct", u8"agggta[cgt]a|t[acg]taccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=agggta[cgt]a|t[acg]taccct", u8"agggta[cgt]a|t[acg]taccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=agggta[cgt]a|t[acg]taccct", u8"agggta[cgt]a|t[acg]taccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=agggta[cgt]a|t[acg]taccct", u8"agggta[cgt]a|t[acg]taccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=agggta[cgt]a|t[acg]taccct", u8"agggta[cgt]a|t[acg]taccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=agggta[cgt]a|t[acg]taccct", u8"agggta[cgt]a|t[acg]taccct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=agggt[cgt]aa|tt[acg]accct", u8"agggt[cgt]aa|tt[acg]accct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=agggt[cgt]aa|tt[acg]accct", u8"agggt[cgt]aa|tt[acg]accct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=agggt[cgt]aa|tt[acg]accct", u8"agggt[cgt]aa|tt[acg]accct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=agggt[cgt]aa|tt[acg]accct", u8"agggt[cgt]aa|tt[acg]accct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=agggt[cgt]aa|tt[acg]accct", u8"agggt[cgt]aa|tt[acg]accct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=agggt[cgt]aa|tt[acg]accct", u8"agggt[cgt]aa|tt[acg]accct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BENCH_RE2, u8"p=ag[act]gtaaa|tttac[agt]ct", u8"ag[act]gtaaa|tttac[agt]ct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_DFA, u8"p=ag[act]gtaaa|tttac[agt]ct", u8"ag[act]gtaaa|tttac[agt]ct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_BOOST, u8"p=ag[act]gtaaa|tttac[agt]ct", u8"ag[act]gtaaa|tttac[agt]ct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p=ag[act]gtaaa|tttac[agt]ct", u8"ag[act]gtaaa|tttac[agt]ct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p=ag[act]gtaaa|tttac[agt]ct", u8"ag[act]gtaaa|tttac[agt]ct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p=ag[act]gtaaa|tttac[agt]ct", u8"ag[act]gtaaa|tttac[agt]ct", ZRegex::CodegenOpts() /*encoding*/, DS_REGEX_DNA)->Unit(benchmark::kMicrosecond);

