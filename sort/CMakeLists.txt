file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(sort ${SOURCES})

target_include_directories(sort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../tape)
