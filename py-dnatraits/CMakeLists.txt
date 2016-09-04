cmake_minimum_required(VERSION 3.5)
project(pydnatraits CXX)
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
) # TODO: find package on dnatraits

#target_link_libraries(pydnatraits ${PYTHON_LIBRARIES})

add_library(pydnatraits SHARED ${sources})
target_link_libraries(pydnatraits python2.7 dnatraits)
add_dependencies(pydnatraits dnatraits)
