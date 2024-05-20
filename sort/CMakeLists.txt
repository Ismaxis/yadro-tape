file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(sort ${SOURCES})

target_link_libraries(sort tape)
target_include_directories(sort PUBLIC ${CMAKE_HOME_DIRECTORY}/tape)
