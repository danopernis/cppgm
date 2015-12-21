cmake_minimum_required(VERSION 3.10)

project(cppgm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O3 -march=native -g -Wall -Wextra -pedantic -Werror -pedantic-errors")
#set(CMAKE_CXX_CLANG_TIDY clang-tidy-9 -checks=-*,readability-*)

add_subdirectory(libcompiler)

add_executable(pptoken pptoken.cpp)
target_link_libraries(pptoken compiler)

enable_testing()
add_subdirectory(test)
