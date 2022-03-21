import json


def generate_main():
    return """
    int main(int argc, char** argv) {
      ::benchmark::Initialize(&argc, argv);
      spdlog::cfg::load_argv_levels(argc, argv);
      ::benchmark::RunSpecifiedBenchmarks();
      ::benchmark::Shutdown();
      return 0;
    }
    """


def gen_simd(ds, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, u8"p={pattern}", u8"{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    if timescale != "ns":
        if timescale == "ms":
            r += "->Unit(benchmark::kMillisecond)"
        elif timescale == "us":
            r += "->Unit(benchmark::kMicrosecond)"

    return r + ";\n"


def gen_kmp(ds, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_KMP_DFA, u8"p={pattern}", u8"{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)'
    if timescale != "ns":
        if timescale == "ms":
            r += "->Unit(benchmark::kMillisecond)"
        elif timescale == "us":
            r += "->Unit(benchmark::kMicrosecond)"

    r2 = f'BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, u8"p={pattern}", u8"{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    if timescale != "ns":
        if timescale == "ms":
            r2 += "->Unit(benchmark::kMillisecond)"
        elif timescale == "us":
            r2 += "->Unit(benchmark::kMicrosecond)"

    return r + ";\n" + r2 + ";\n"


def gen_dfa(ds, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_DFA, u8"p={pattern}", u8"{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)'
    if timescale != "ns":
        if timescale == "ms":
            r += "->Unit(benchmark::kMillisecond)"
        elif timescale == "us":
            r += "->Unit(benchmark::kMicrosecond)"

    return r + ";\n"


def gen_re2(ds, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_RE2, u8"p={pattern}", u8"{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    if timescale != "ns":
        if timescale == "ms":
            r += "->Unit(benchmark::kMillisecond)"
        elif timescale == "us":
            r += "->Unit(benchmark::kMicrosecond)"

    return r + ";\n"


def gen_boost(ds, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_BOOST, u8"p={pattern}", u8"{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    if timescale != "ns":
        if timescale == "ms":
            r += "->Unit(benchmark::kMillisecond)"
        elif timescale == "us":
            r += "->Unit(benchmark::kMicrosecond)"

    return r + ";\n"


def generate_fn(ds: str, timescale: str, libraries: str, pattern: str):
    result = ""
    for lib in libraries:
        if lib == "re2":
            result += gen_re2(ds, pattern, timescale)
        elif lib == "dfa":
            result += gen_dfa(ds, pattern, timescale)
        elif lib == "kmp":
            result += gen_kmp(ds, pattern, timescale)
        elif lib == "simd":
            result += gen_simd(ds, pattern, timescale)
        else:
            result += gen_boost(ds, pattern, timescale)
    return result


def main(file: str):
    with open(file) as json_file:
        data = json.load(json_file)
    suites = data["Suites"]
    includes = []
    for suite in suites:
        name, ds, timescale, libraries = (
            suite["name"],
            suite["dataset"],
            suite.get("timescale", "ns"),
            suite.get("libraries", ["re2", "dfa", "boost", "kmp"]),
        )
        ds_name = f"DS_{name.upper()}"
        with open(f"../source/suites/{name}.hpp", "w") as f:
            includes.append(f'#include "suites/{name}.hpp"\n')
            f.write(f'#include "bench.h"\n')
            f.write(f'#define {ds_name} "{ds}"\n')
            for pattern in suite["patterns"]:
                f.write(
                    generate_fn(
                        ds_name,
                        timescale=timescale,
                        libraries=libraries,
                        pattern=pattern,
                    )
                    + "\n"
                )
        with open(f"../source/main.cpp", "w") as f:
            f.writelines(includes)
            f.write(generate_main() + "\n")


if __name__ == "__main__":
    main("./ascii_bench.json")
