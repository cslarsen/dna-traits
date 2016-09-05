cmake_minimum_required(VERSION 3.5)
project(_dna_traits CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB sources
  LIST_DIRECTORIES false
  ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(PythonLibs 2 REQUIRED)
find_package(PythonInterp 2 REQUIRED)

include_directories(
  ${PROJECT_SOURCE_DIR}/src # only the single .hpp file
  ${PYTHON_INCLUDE_DIRS}
  ${dnatraits_INCLUDE_DIR}
) # TODO: use package semantics for dnatraits?

add_library(_dna_traits SHARED ${sources})

target_link_libraries(_dna_traits
  ${PYTHON_LIBRARIES}
  dnatraits
)

# Python modules requires to be named "foo.so"
set_target_properties(_dna_traits
  PROPERTIES
    PREFIX ""
    SUFFIX ".so")

file(GLOB pysources
  LIST_DIRECTORIES false
  ${PROJECT_SOURCE_DIR}/dna_traits/*.py)
file(GLOB pyreports
  LIST_DIRECTORIES false
  ${PROJECT_SOURCE_DIR}/dna_traits/reports/*.py)

add_dependencies(_dna_traits dnatraits)

install(FILES ${pysources} DESTINATION dna_traits)
install(FILES ${pyreports} DESTINATION dna_traits/reports)
install(TARGETS _dna_traits LIBRARY DESTINATION dna_traits)
install(PROGRAMS ${PROJECT_SOURCE_DIR}/bin/dnatraits DESTINATION bin)
