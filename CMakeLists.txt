cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
project(dnatraits-all CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# TODO: if release mode ...
# TODO: See if we can get CMake to set these flags automatically
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

add_subdirectory(dnatraits)
add_subdirectory(py-dnatraits)
