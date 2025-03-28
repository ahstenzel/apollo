cmake_minimum_required(VERSION 3.16)
project(apollo VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

add_subdirectory(vendor)
add_subdirectory(src)