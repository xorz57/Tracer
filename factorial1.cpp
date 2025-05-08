#include <iostream>

#include "tracing.hpp"

std::uint64_t factorial(std::uint64_t n) {
    const tracing::DurationEvent event{"factorial", { {"n", n} }};
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    std::cout << factorial(10) << std::endl;
    tracing::Tracer::get_instance().dump("trace.json", 4);
}
