cmake_minimum_required(VERSION 3.24)

project(tracing)

add_library(tracing INTERFACE)
target_include_directories(tracing INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(factorial1 examples/factorial1.cpp)
target_link_libraries(factorial1 PRIVATE tracing)
target_compile_features(factorial1 PRIVATE cxx_std_11)

add_executable(factorial2 examples/factorial2.cpp)
target_link_libraries(factorial2 PRIVATE tracing)
target_compile_features(factorial2 PRIVATE cxx_std_11)

add_executable(fibonacci1 examples/fibonacci1.cpp)
target_link_libraries(fibonacci1 PRIVATE tracing)
target_compile_features(fibonacci1 PRIVATE cxx_std_11)

add_executable(fibonacci2 examples/fibonacci2.cpp)
target_link_libraries(fibonacci2 PRIVATE tracing)
target_compile_features(fibonacci2 PRIVATE cxx_std_11)
