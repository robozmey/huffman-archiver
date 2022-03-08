cmake_minimum_required(VERSION 3.10.2)
project(hw_02)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wreorder")

include_directories("src/")

include_directories("test/")

add_library(huffman STATIC src/huffman.cpp src/huffman.h)

add_executable(hw_02 src/main.cpp)
target_link_libraries(hw_02 huffman)

add_executable(hw_02_test test/test.cpp test/huffman_test.cpp test/huffman_test.h)
target_link_libraries(hw_02_test huffman)