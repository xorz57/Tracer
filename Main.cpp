#include <iostream>

#include "Tracer.hpp"

std::uint64_t factorial(std::uint64_t n) {
	ScopedTracer _("factorial", "function", { {"n", n} });
	if (n <= 1) return 1;
	return n * factorial(n - 1);
}

std::uint64_t fibonacci(std::uint64_t n) {
	ScopedTracer _("fibonacci", "function", { {"n", n} });
	if (n <= 1) return n;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

std::uint64_t factorial2(std::uint64_t n) {
	ScopedTracer _("factorial2", "function", { {"n", n} });
	std::uint64_t result = 1;
	for (std::uint64_t i = 2; i <= n; ++i) {
		result *= i;
	}
	return result;
}

std::uint64_t fibonacci2(std::uint64_t n) {
	ScopedTracer tracer("fibonacci2", "function", { {"n", n} });
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
	std::cout << factorial(10) << std::endl;
	std::cout << fibonacci(10) << std::endl;
	std::cout << factorial2(10) << std::endl;
	std::cout << fibonacci2(10) << std::endl;
	Tracer::getInstance().dump("trace.json", 4);
}
