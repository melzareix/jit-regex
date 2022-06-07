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


def add_timescale(r, timescale):
    if timescale == "ms":
        r += "->Unit(benchmark::kMillisecond)"
    elif timescale == "us":
        r += "->Unit(benchmark::kMicrosecond)"
    else:
        r += "->Unit(benchmark::kSecond)"
    return r


def gen_simd_multi(ds, name: str, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_SIMD_MULTIPATTERN, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    r = add_timescale(r, timescale)
    return r + ";\n"


def gen_simd(ds, name: str, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_SIMD_INTERPRETTED, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    r = add_timescale(r, timescale)
    return r + ";\n"


def gen_kmp(ds, name: str, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_KMP_DFA, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})->DenseRange(KMP_TYPE_LLVM, KMP_TYPE_CPP)'
    r = add_timescale(r, timescale)

    r2 = f'BENCHMARK_CAPTURE(BENCH_KMP_INTERPRETTED, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    r2 = add_timescale(r, timescale)

    return r + ";\n" + r2 + ";\n"


def gen_dfa(ds, name: str, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_DFA, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)'
    r = add_timescale(r, timescale)

    return r + ";\n"


def gen_re2(ds, name: str, pattern: str, timescale="ns"):
    pattern = pattern.replace("%", ".*")
    r = f'BENCHMARK_CAPTURE(BENCH_RE2, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    r = add_timescale(r, timescale)

    return r + ";\n"


def gen_pcre2(ds, name: str, pattern: str, timescale="ns"):
    pattern = pattern.replace("%", ".*")
    r = f'BENCHMARK_CAPTURE(BENCH_PCRE2, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    r = add_timescale(r, timescale)

    return r + ";\n"


def gen_boost(ds, name: str, pattern, timescale="ns"):
    r = f'BENCHMARK_CAPTURE(BENCH_BOOST, "p={name}", "{pattern}", ZRegex::CodegenOpts() /*encoding*/, {ds})'
    r = add_timescale(r, timescale)

    return r + ";\n"


def generate_fn(ds: str, name: str, timescale: str, libraries: str, pattern: str):
    result = ""
    for lib in libraries:
        if lib == "re2":
            result += gen_re2(ds, name, pattern, timescale)
        elif lib == "dfa":
            result += gen_dfa(ds, name, pattern, timescale)
        elif lib == "kmp":
            result += gen_kmp(ds, name, pattern, timescale)
        elif lib == "simd_multi":
            result += gen_simd_multi(ds, name, pattern, timescale)
        elif lib == "simd":
            result += gen_simd(ds, name, pattern, timescale)
        elif lib == "pcre2":
            result += gen_pcre2(ds, name, pattern, timescale)
        else:
            result += gen_boost(ds, name, pattern, timescale)
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
            suite.get("libraries", ["re2", "pcre2", "dfa"]),
        )
        ds_name = f"DS_{name.upper()}"
        with open(f"../source/suites/{name}.hpp", "w") as f:
            includes.append(f'#include "suites/{name}.hpp"\n')
            f.write(f'#include "bench.h"\n')
            f.write(f'#define {ds_name} "{ds}"\n')
            i = 0
            for pattern in suite["patterns"]:
                f.write(
                    generate_fn(
                        ds_name,
                        f"{i}",
                        timescale=timescale,
                        libraries=libraries,
                        pattern=pattern,
                    )
                    + "\n"
                )
                i += 1
        with open(f"../source/main.cpp", "w") as f:
            f.writelines(includes)
            f.write(generate_main() + "\n")


if __name__ == "__main__":
    main("./ascii_bench.json")
