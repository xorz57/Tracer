#include <iostream>

#include "tracing.hpp"

std::uint64_t fibonacci(std::uint64_t n) {
    const tracing::DurationEvent event{"fibonacci", { {"n", n} }};
    if (n <= 1) return n;
    std::uint64_t a = 0;
    std::uint64_t b = 1;
    for (std::uint64_t i = 2; i <= n; ++i) {
        std::uint64_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    std::cout << fibonacci(10) << std::endl;
    tracing::Tracer::get_instance().dump("trace.json", 4);
}
