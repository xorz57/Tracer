#include <iostream>

#include "tracing.hpp"

auto fibonacci(std::uint64_t n) -> std::uint64_t {
  const tracing::DurationEvent event{"fibonacci"};
  if (n <= 1)
    return n;
  std::uint64_t a = 0;
  std::uint64_t b = 1;
  for (std::uint64_t i = 2; i <= n; ++i) {
    std::uint64_t c = a + b;
    a = b;
    b = c;
  }
  return b;
}

auto main() -> int {
  std::cout << fibonacci(20) << std::endl;
  tracing::Tracer::get_instance().dump("trace.json", 4);
}
