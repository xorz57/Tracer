#include <iostream>

#include "tracing.hpp"

auto fibonacci(std::uint64_t n) -> std::uint64_t {
  const tracing::DurationEvent event{"fibonacci"};
  if (n <= 1)
    return n;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

auto main() -> int {
  std::cout << fibonacci(20) << std::endl;
  tracing::Tracer::get_instance().dump("trace.json", 4);
}
