cmake_minimum_required(VERSION 3.5)
project(dnatraits CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB sources
  LIST_DIRECTORIES true
  ${PROJECT_SOURCE_DIR}/src/*)

list(APPEND
  CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake")

find_package(google_densehash REQUIRED)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${google_densehash_INCLUDE_DIR}
)

# TODO: if release mode ...
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")

add_library(dnatraits SHARED ${sources})
