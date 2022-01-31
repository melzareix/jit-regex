#include <doctest/doctest.h>
#include <zregex/version.h>

#include <string>

TEST_CASE("ZRegex version") {
  static_assert(std::string_view(ZREGEX_VERSION) == std::string_view("1.0"));
  CHECK(std::string(ZREGEX_VERSION) == std::string("1.0"));
}
