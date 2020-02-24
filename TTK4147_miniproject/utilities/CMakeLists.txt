
cmake_minimum_required (VERSION 3.0)
project(Sanntid_oving_4)

include_directories (src/ )



file (GLOB_RECURSE PROJECT_HEADERS src/*.h  )
file (GLOB_RECURSE PROJECT_SOURCES src/*.c )
file (GLOB         PROJECT_CONFIGS CMakeLists.txt
                                  .gitmodules)





source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})

# set(SELF_MADE_FLAGS " -std=gnu11 -pthread -lm -lcomedi")
set(SELF_MADE_FLAGS " -std=gnu11 -pthread -lm")

add_definitions(${SELF_MADE_FLAGS})
# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${SELF_MADE_FLAGS}")
SET(CMAKE_CXX_FLAGS  " ${CMAKE_CXX_FLAGS} ${SELF_MADE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${SELF_MADE_FLAGS}")

add_definitions (-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_CONFIGS})
target_link_libraries (${PROJECT_NAME})
set_target_properties (${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})