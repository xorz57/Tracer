#include <iostream>

#include "Tracer.hpp"

std::uint64_t factorial(std::uint64_t n) {
	TRACER_SCOPE("factorial", "function", { {"n", n} });
	if (n <= 1) return 1;
	return n * factorial(n - 1);
}

int main() {
	std::cout << factorial(10) << std::endl;
	Tracer::getInstance().dump("trace.json", 4);
}
