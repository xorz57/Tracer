#include <iostream>

#include "Tracer.hpp"

std::uint64_t factorial(std::uint64_t n) {
	TRACER_SCOPE("factorial", "function", { {"n", n} });
	std::uint64_t result = 1;
	for (std::uint64_t i = 2; i <= n; ++i) {
		result *= i;
	}
	return result;
}

int main() {
	std::cout << factorial(10) << std::endl;
	Tracer::getInstance().dump("trace.json", 4);
}
