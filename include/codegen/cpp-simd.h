#pragma once
#include <fstream>
#include <iostream>

#include "spdlog/fmt/fmt.h"

// void JIT_cpp() {
//   llvm::SMDiagnostic Err;
//   std::string fnName;

//   module = llvm::parseIRFile(fmt::format("/tmp/{}.ll", filename_), Err, *context.getContext());
//   for (auto& m : module->getFunctionList()) {
//     auto fn = m.getName().str();
//     spdlog::debug("Fn {}", fn);
//     if (fn.find("traverse") != std::string::npos) {
//       fnName = fn;
//       break;
//     }
//   }
//   auto err = jit->addModule(std::move(module));
//   traverse_ptr = reinterpret_cast<bool (*)(char*, uint32_t
//   n)>(jit->getPointerToFunction(fnName));
// }
// void CompileSIMDCode(std::string pattern) {
//   std::ofstream fs;
//   // file is temporary compiled
//   std::string filename = "simd-regex";
//   auto fp = fmt::format("/tmp/{}.cpp", filename);
//   fs.open(fp);
//   GenerateSIMDCode(fs, pattern);

//   auto cmd = fmt::format(
//       "cd /tmp && clang-13 -std=c++14 -march=native -O3 -emit-llvm {}.cpp -o {}.ll -S", filename,
//       filename);
//   auto exit_code = system(cmd.c_str());
// }
void GenerateSIMDCode(std::ofstream& fs, std::string& needle) {
  fs << "#include <x86intrin.h>" << std::endl;
  fs << "#include <cstring>" << std::endl;
  fs << "#include <cstdint>" << std::endl;
  fs << "bool str_str(const char *s, size_t n) {" << std::endl;
  fs << fmt::format("const size_t k = {};", needle.size()) << std::endl;
  //   fs << fmt::format("const char* needle = \"{}\";", needle) << std::endl;

  fs << fmt::format("  const __m256i first = _mm256_set1_epi8('{}');", needle[0]) << std::endl;
  fs << fmt::format("  const __m256i last = _mm256_set1_epi8('{}');", needle[needle.size() - 1])
     << std::endl;

  fs << "  for (size_t i = 0; i < n; i += 32) {" << std::endl;
  fs << "    const __m256i block_first = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s "
        "+ i));"
     << std::endl;
  fs << "    const __m256i block_last" << std::endl;
  fs << "        = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s + i + k - 1));"
     << std::endl;

  fs << "    const __m256i eq_first = _mm256_cmpeq_epi8(first, block_first);" << std::endl;
  fs << "    const __m256i eq_last = _mm256_cmpeq_epi8(last, block_last);" << std::endl;

  fs << "    uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(eq_first, eq_last));"
     << std::endl;

  fs << "    while (mask != 0) {" << std::endl;
  fs << "      const auto bitpos = __builtin_ctz(mask);" << std::endl;

  fs << fmt::format(
      "if ((i + bitpos + k - 1) < n && std::memcmp(s + i + bitpos + 1, \"{}\", {}) == 0) ",
      needle.substr(1), needle.size() - 2)
     << std::endl;
  fs << "        { return true;" << std::endl;
  fs << "      }" << std::endl;

  fs << "      mask = mask & (mask - 1);" << std::endl;
  fs << "    }" << std::endl;
  fs << "  }" << std::endl;

  fs << "  return false;" << std::endl;
  fs << "  }" << std::endl;
}