#include <iostream>

#include "tracing.hpp"

auto factorial(std::uint64_t n) -> std::uint64_t {
  if (n <= 1) {
    tracing::instant_event("factorial.base_case");
    return 1;
  }
  tracing::DurationEvent const event{"factorial.recursive_call"};
  return n * factorial(n - 1);
}

auto main() -> int {
  std::cout << factorial(20) << std::endl;
  tracing::Tracer::get_instance().dump("trace.json");
}
