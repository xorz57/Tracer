#include <iostream>

#include "tracing.hpp"

std::uint64_t factorial(std::uint64_t n) {
    const tracing::DurationEvent event{"factorial", { {"n", n} }};
    std::uint64_t result = 1;
    for (std::uint64_t i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    std::cout << factorial(10) << std::endl;
    tracing::Tracer::get_instance().dump("trace.json", 4);
}
