project(constexpr-conway)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(
  -Wall
  -Wextra
  -Werror
  -Wnull-dereference
  -Wold-style-cast
  -Wdouble-promotion
  -Wshadow)

include_directories(include)
add_executable(conway main.cpp)
