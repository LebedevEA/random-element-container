cmake_minimum_required(VERSION 3.16.3)
project(random_element_container)

set(CMAKE_CXX_STANDARD 17)

include_directories("include" "tests")

add_executable(random_element_container include/is_hashable.h include/hashing_impl.h include/non_hashing_impl.h include/random_container.h tests/main_tests.cpp tests/utils.h)
