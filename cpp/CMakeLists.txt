cmake_minimum_required(VERSION 3.11)
project(optools)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++17 -O2")
include_directories(src/)
add_subdirectory(src/mono)
add_subdirectory(src/utils)

add_executable(main "src/main.cpp")
add_executable(test "src/test.cpp")

target_link_libraries(main mono utils pthread)
target_link_libraries(test utils pthread)