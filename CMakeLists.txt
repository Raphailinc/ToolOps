cmake_minimum_required(VERSION 3.16)
project(toolops LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(toolops STATIC
    src/storage.cpp
    src/domain.cpp
)
target_include_directories(toolops PUBLIC src)

add_executable(toolops_cli src/main.cpp)
target_link_libraries(toolops_cli PRIVATE toolops)

enable_testing()
add_executable(toolops_tests tests/main.cpp tests/test_core.cpp)
target_link_libraries(toolops_tests PRIVATE toolops)
add_test(NAME toolops_tests COMMAND toolops_tests)
