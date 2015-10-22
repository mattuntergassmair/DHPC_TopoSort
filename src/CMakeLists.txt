cmake_minimum_required(VERSION 2.8)

# Manually set CXX Compiler, CC Compiler. This can't be done later with ccmake!
set(CMAKE_C_COMPILER   "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(TopoSort)

# Manually set CXX Flags
set(CXX_FLAGS
	"-DNDEBUG -O3"
	CACHE STRING
	"Additional CXX-Compiler flags. Use Build Type None if you want to have full control over the flags."
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

## Macro definitions
# make CMAKE_PROJECT_SOURCE_DIR accessible in source code via predefined constant CMAKE_PROJECT_SOURCE_DIR
if(CMAKE_SOURCE_DIR)
  add_definitions(-D'CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"')
endif()

# define the correct directory separator for windows / unix
if(WIN32) 
  add_definitions(-D'DS=\"\\\\\"') # masking mess
else()
  add_definitions(-D'DS=\"/\"')
endif()

## Files
set(
  SRC_SOURCES
  "graph.cpp"
  "graphdoc.cpp"
  "main_toposort.cpp"
  "node.cpp"
)

add_executable(toposort.exe ${SRC_SOURCES})
