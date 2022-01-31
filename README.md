# ZRegex

A JIT Compiled SQL Compliant Regular Expressions Engine.

**WIP** for my master thesis at Technical University of Munich.

## Usage

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/ZRegex --help
```

### Build and run benchmarks test suite

Use the following commands from the project's root directory to run the benchmark suite.

```bash
cmake -S test -B build/benchmark
cmake --build build/benchmark
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/benchmark --target benchmark
```

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -S test -B build/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details.