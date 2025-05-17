#include <iostream>

#include "tracing.hpp"

auto factorial(std::uint64_t n) -> std::uint64_t {
  tracing::DurationEvent const event{"factorial"};
  std::uint64_t result = 1;
  for (std::uint64_t i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

auto main() -> int {
  std::cout << factorial(20) << std::endl;
  tracing::Tracer::get_instance().dump("trace.json");
}
