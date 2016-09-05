cmake_minimum_required(VERSION 3.5)
project(dnatraits CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB sources
  LIST_DIRECTORIES false
  ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(google_densehash REQUIRED)

set(dnatraits_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)
set(dnatraits_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include) # for this scope

include_directories(
  ${dnatraits_INCLUDE_DIR}
  ${google_densehash_INCLUDE_DIR}
)

add_library(dnatraits STATIC ${sources})

set_target_properties(dnatraits
  PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1)
