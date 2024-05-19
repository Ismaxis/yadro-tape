cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE D:/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_TRIPLET x64-mingw-static)
project(tape)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(tests)

add_executable(tape main.cpp)
