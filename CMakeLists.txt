cmake_minimum_required(VERSION 3.15)
project(long_numbers)

set(CMAKE_CXX_STANDARD 14)

add_executable(long_numbers main.cpp BigInt.cpp BigInt.h)