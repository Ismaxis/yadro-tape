cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE D:/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_TRIPLET x64-mingw-static)
project(tape)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(util)
add_subdirectory(tape)
add_subdirectory(sort)
add_subdirectory(tests)

find_package(gflags CONFIG REQUIRED)

add_executable(tape-main main.cpp)
target_link_libraries(tape-main gflags::gflags sort tape util)
target_include_directories(tape-main PRIVATE ${CMAKE_HOME_DIRECTORY})
