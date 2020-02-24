import sys 


arguments =sys.argv
expandFactor = arguments[1]

String = """cmake_minimum_required (VERSION 2.6)
project (CMAKE_PROGRAM)

include_directories(src/)

file(GLOB SOURCES "src/*.c")

add_definitions(-DEXPAND_FACTOR="""+ expandFactor+""")
add_executable(helloWorld ${SOURCES})
"""
print(String)