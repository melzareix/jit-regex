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


def generate_fn(ds: str, mode: str, pattern: str, line_by_line: bool = True):
    read_all = "LINE" if line_by_line else "ALL"
    f = (
        f"ZRegex::Encoding::UTF8"
        if mode == "UTF8"
        else f"ZRegex::Encoding::UTF8::ASCII"
    )
    f_name = f"BENCH_{read_all}" if mode == "UTF8" else f"BENCH_ASCII"
    return f"""BENCHMARK_CAPTURE({f_name}, u8"Unicode, p={pattern}", u8"{pattern}" /*pattern*/, true /*partial*/,  {f} /*encoding*/, "/Users/melzarei/Desktop/bench_data/{ds}")->DenseRange(DFA_LLVM, RE2_TYPE)->Unit(benchmark::kMillisecond);"""


def main(file: str):
    with open(file) as json_file:
        data = json.load(json_file)
    suites = data["Suites"]
    with open("test.cpp", "w") as f:
        for suite in suites:
            for test in suite["tests"]:
                pattern, ds = test["pattern"], test["dataset"]
                f.write(
                    generate_fn(ds, mode="UTF8", pattern=pattern, line_by_line=True)
                    + "\n"
                )
        f.write(generate_main() + "\n")


if __name__ == "__main__":
    main("./ascii_bench.json")
