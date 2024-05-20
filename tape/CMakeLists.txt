file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(tape ${SOURCES})

target_include_directories(tape PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../bin_util)
