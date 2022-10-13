cmake_minimum_required(VERSION 3.22)
project(demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(demo src/main.cpp src/Parser_regex.cpp include/Parser_regex.h)
