cmake_minimum_required(VERSION 3.22)
project(demo)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(demo
        src/main.cpp
        src/parser_regex.cpp)