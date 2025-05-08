# tracing

## Dependencies

- [nlohmann/json](https://github.com/nlohmann/json)

## Examples

### Factorial 1

```cpp
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
```

![Factorial1](https://github.com/user-attachments/assets/3f2b0870-ae5c-4786-85a9-1583197797a6)

### Fibonacci 1

```cpp
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
```

![Fibonacci1](https://github.com/user-attachments/assets/bce1ad72-11a7-4a49-9e34-bd3d976e796a)

### Factorial 2

```cpp
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
```

![Factorial2](https://github.com/user-attachments/assets/8c53255f-c0e8-4473-aa7b-c9cf5ea86cdb)

### Fibonacci 2

```cpp
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
```

![Fibonacci2](https://github.com/user-attachments/assets/91437b78-bef8-468c-be4e-6b6d9954930b)
