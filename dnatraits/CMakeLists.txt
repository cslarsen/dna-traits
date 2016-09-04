cmake_minimum_required(VERSION 3.5)
project(dnatraits CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB sources
  LIST_DIRECTORIES true
  ${PROJECT_SOURCE_DIR}/src/*)

list(APPEND
  CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake")

find_package(google_densehash REQUIRED)

set(dnatraits_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)
set(dnatraits_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include) # for this scope

include_directories(
  ${dnatraits_INCLUDE_DIR}
  ${google_densehash_INCLUDE_DIR}
)

# TODO: if release mode ...
# TODO: See if we can get CMake to set these flags automatically
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

add_library(dnatraits SHARED ${sources})
