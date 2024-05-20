cmake_minimum_required(VERSION 3.20)
project(tape-tests)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

find_package(GTest REQUIRED)

file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(tests ${TESTS})

target_link_libraries(tests GTest::gtest GTest::gtest_main sort tape util)
target_include_directories(tests PUBLIC 
    ${CMAKE_HOME_DIRECTORY}/tape
    ${CMAKE_HOME_DIRECTORY}/sort
    ${CMAKE_HOME_DIRECTORY}/util
    )
gtest_discover_tests(tests)
