#include <fmt/format.h>
#include <greeter/greeter.h>
#include <re2/re2.h>

using namespace greeter;

Greeter::Greeter(std::string _name) : name(std::move(_name)) {}

std::string Greeter::greet(LanguageCode lang) const {
   int matchResult = RE2::FullMatch("hello", "h.*o"); 

  switch (lang) {
    default:
    case LanguageCode::EN:
      return fmt::format("Hello, {} {}!", name, matchResult);
    case LanguageCode::DE:
      return fmt::format("Hallo {}!", name);
    case LanguageCode::ES:
      return fmt::format("¡Hola {}!", name);
    case LanguageCode::FR:
      return fmt::format("Bonjour {}!", name);
  }
}
