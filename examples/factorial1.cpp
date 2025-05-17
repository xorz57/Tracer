#include <iostream>

#include <xorz57/tracing.hpp>

auto factorial(std::uint64_t n) -> std::uint64_t {
  if (n <= 1) {
    xorz57::tracing::instant_event("factorial.base_case");
    return 1;
  }
  xorz57::tracing::DurationEvent const event{"factorial.recursive_call"};
  return n * factorial(n - 1);
}

auto main() -> int {
  std::cout << factorial(20) << std::endl;
  xorz57::tracing::Tracer::get_instance().dump("trace.json");
}
