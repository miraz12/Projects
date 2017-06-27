cmake_minimum_required(VERSION 3.5)
project(AStar)
FILE(GLOB Paradox_headers *.h)
FILE(GLOB Paradox_sources *.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -static -std=gnu++11")

set(SOURCE_FILES ${Paradox_sources} ${Paradox_headers})
add_executable(AStar ${SOURCE_FILES})

source_group("source" FILES  ${Paradox_sources})
source_group("header" FILES  ${Paradox_headers})
