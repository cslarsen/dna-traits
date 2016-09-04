cmake_minimum_required(VERSION 3.5)
project(_dna_traits CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB sources
  LIST_DIRECTORIES true
  ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(Python_ADDITIONAL_VERSIONS 2.7)
find_package(PythonLibs REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/../dnatraits/include
) # TODO: use package semantics for dnatraits?

add_library(_dna_traits SHARED ${sources})
target_link_libraries(_dna_traits
  ${PYTHON_LIBRARIES}
  dnatraits
)

add_dependencies(_dna_traits dnatraits)
