#include <iostream>

#include <xorz57/tracing.hpp>

auto fibonacci(std::uint64_t n) -> std::uint64_t {
  xorz57::tracing::DurationEvent const event{"fibonacci"};
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
  xorz57::tracing::Tracer::get_instance().dump("trace.json");
}
