#include <iostream>

#include <xorz57/tracing.hpp>

auto fibonacci(std::uint64_t n) -> std::uint64_t {
  if (n <= 1) {
    xorz57::tracing::instant_event("fibonacci.base_case");
    return n;
  }
  xorz57::tracing::DurationEvent const duration{"fibonacci.recursive_call"};
  return fibonacci(n - 1) + fibonacci(n - 2);
}

auto main() -> int {
  std::cout << fibonacci(20) << std::endl;
  xorz57::tracing::Tracer::get_instance().dump("trace.json");
}
