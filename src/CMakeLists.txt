cmake_minimum_required(VERSION 2.8)

# Manually set CXX Compiler, CC Compiler and flags. This can't be done later with ccmake!
if(UNIX)
  set(CMAKE_C_COMPILER   "gcc")
  set(CMAKE_CXX_COMPILER "g++")  
endif()

project(TopoSort)

if(UNIX)
# Manually set CXX Flags
set(CXX_FLAGS
    "-std=c++11 -fopenmp"
    CACHE STRING
    "Additional CXX-Compiler flags. Use Build Type None if you want to have full control over the flags."
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")
endif()

## Macro definitions
# make CMAKE_PROJECT_SOURCE_DIR accessible in source code via predefined constant CMAKE_PROJECT_SOURCE_DIR
if(CMAKE_SOURCE_DIR)
  add_definitions(-DCMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")
endif()

## Files
set(
  SRC_SOURCES
  "graph.cpp"
  "graphdoc.cpp"
  "main_toposort.cpp"
  "node.cpp"
)

add_executable(toposort_serial.exe ${SRC_SOURCES} graphsort_serial.cpp)
add_executable(toposort_omp_basic.exe ${SRC_SOURCES} graphsort_omp_basic.cpp)
add_executable(toposort_omp_tasks.exe ${SRC_SOURCES} graphsort_omp_tasks.cpp)
add_executable(toposort_omp_bitset.exe ${SRC_SOURCES} graphsort_omp_bitset.cpp)
