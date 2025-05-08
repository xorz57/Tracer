#include <iostream>

#include "tracing.hpp"

std::uint64_t fibonacci(std::uint64_t n) {
    const tracing::DurationEvent event{"fibonacci", { {"n", n} }};
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    std::cout << fibonacci(10) << std::endl;
    tracing::Tracer::get_instance().dump("trace.json", 4);
}
