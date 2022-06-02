import json
import glob
import os
from pathlib import Path


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


def gen_re2(ds, pattern: str):
    # pattern = pattern.replace("%", ".*")
    # print(pattern)
    r = f'BENCHMARK_CAPTURE(BENCH_RE2, "{pattern}",ZRegex::CodegenOpts() /*encoding*/, {ds})'
    return r + ";\n"


def gen_dfa(ds, ds_name):
    r = f'BENCHMARK_CAPTURE(BENCH_DFA, "{ds_name}", ZRegex::CodegenOpts() /*encoding*/, "{ds}")->DenseRange(DFA_LLVM_U8, DFA_CPP_U32)->Unit(benchmark::kSecond)->UseRealTime()'
    return r + ";\n"


def main(folder: str):
    benches = glob.glob(f"{folder}/*.txt")
    includes = []
    print(benches)
    for file in benches:
        print(file)
        name = Path(file).stem
        with open(f"../source/suites/compile/{name}.hpp", "w") as f:
            includes.append(f'#include "suites/compile/{name}.hpp"\n')
            f.write(f'#include "../benchcmp.hpp"\n')
            f.write(gen_dfa(file, name) + "\n")
            # f.write(gen_re2(file, name) + "\n")
        with open(f"../source/main_cmp.cpp", "w") as f:
            f.writelines(includes)
            f.write(generate_main() + "\n")


if __name__ == "__main__":
    main(os.path.join(os.getcwd(), "..", "patterns"))
