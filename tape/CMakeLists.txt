file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(tape ${SOURCES})

target_link_libraries(tape util)
target_include_directories(tape PRIVATE ${CMAKE_HOME_DIRECTORY}/util)
