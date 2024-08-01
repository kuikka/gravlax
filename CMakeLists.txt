cmake_minimum_required(VERSION 3.19)

project(lox_interpreter CXX)

cmake_policy(SET CMP0135 NEW)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

find_package(fmt CONFIG REQUIRED)

add_subdirectory(interpreter)
