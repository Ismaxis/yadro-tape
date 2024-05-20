cmake_minimum_required(VERSION 3.20)

project(tape)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(util)
add_subdirectory(tape)
add_subdirectory(sort)
add_subdirectory(tests)

find_package(gflags CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)


add_executable(tape-main main.cpp config/delays.cpp)
target_link_libraries(tape-main JsonCpp::JsonCpp gflags::gflags sort tape util)
target_include_directories(tape-main PRIVATE ${CMAKE_HOME_DIRECTORY})
