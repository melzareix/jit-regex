# ZRegex

A JIT Compiled SQL Compliant Regular Expressions Engine.

**WIP** for my master thesis at Technical University of Munich.

## Usage

### Install deps
```bash
sudo apt-get install -y ninja-build
wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && sudo ./llvm.sh 13
mkdir -p /workspaces/antlr
cd /workspaces/antlr && sudo curl -O https://www.antlr.org/download/antlr-4.10.1-complete.jar
```
### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
CC=g++-11 CXX=g++-11 cmake -GNINJA -DANTLR_EXECUTABLE="/workspaces/antlr/antlr-4.10.1-complete.jar" -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/ZRegex --help
```

### Build and run benchmarks test suite

Use the following commands from the project's root directory to run the benchmark suite.

```bash
CC=g++-11 CXX=g++-11 cmake -GNINJA -DANTLR_EXECUTABLE="/workspaces/antlr/antlr-4.10.1-complete.jar" -S benchmark -B build/benchmark
cmake --build build/benchmark
cmake --build build/benchmark
./build/standalone/ZRegex_BM
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