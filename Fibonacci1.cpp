#include <iostream>

#include "Tracer.hpp"

std::uint64_t fibonacci(std::uint64_t n) {
	TRACER_SCOPE("fibonacci", "function", { {"n", n} });
	if (n <= 1) return n;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
	std::cout << fibonacci(10) << std::endl;
	TRACER_DUMP("trace.json", 4);
}
