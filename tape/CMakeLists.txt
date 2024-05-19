file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/inmemory/*.cpp)

add_library(tape ${SOURCES})

target_include_directories(tape PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
