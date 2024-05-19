cmake_minimum_required(VERSION 3.20)
project(tape-tests)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

find_package(GTest REQUIRED)

add_executable(tests tape_test.cpp sort_test.cpp)

target_link_libraries(tests GTest::gtest GTest::gtest_main tape sort)
target_include_directories(tests PRIVATE 
    ${CMAKE_HOME_DIRECTORY}/tape
    ${CMAKE_HOME_DIRECTORY}/sort
    )
gtest_discover_tests(tests)
