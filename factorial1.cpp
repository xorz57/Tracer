#include <iostream>

#include "tracing.hpp"

auto factorial(std::uint64_t n) -> std::uint64_t {
  const tracing::DurationEvent event{"factorial", {{"n", n}}};
  if (n <= 1)
    return 1;
  return n * factorial(n - 1);
}

auto main() -> int {
  std::cout << factorial(20) << std::endl;
  tracing::Tracer::get_instance().dump("trace.json", 4);
}
