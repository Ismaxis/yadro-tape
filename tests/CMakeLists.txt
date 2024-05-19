cmake_minimum_required(VERSION 3.20)
project(tape-tests)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

find_package(GTest REQUIRED)

add_executable(tests assert_true.cpp)

target_link_libraries(tests GTest::gtest GTest::gtest_main)
gtest_discover_tests(tests)
