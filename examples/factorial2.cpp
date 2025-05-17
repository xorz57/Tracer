#include <iostream>

#include <xorz57/tracing.hpp>

auto factorial(std::uint64_t n) -> std::uint64_t {
  xorz57::tracing::DurationEvent const event{"factorial"};
  std::uint64_t result = 1;
  for (std::uint64_t i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

auto main() -> int {
  std::cout << factorial(20) << std::endl;
  xorz57::tracing::Tracer::get_instance().dump("trace.json");
}
