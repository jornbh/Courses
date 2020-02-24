cmake_minimum_required (VERSION 2.6)
project (CMAKE_PROGRAM)

include_directories(src/)

file(GLOB SOURCES "src/*.c")

add_definitions(-DEXPAND_FACTOR=7)
add_executable(helloWorld ${SOURCES})

