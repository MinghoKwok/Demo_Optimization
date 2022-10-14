cmake_minimum_required(VERSION 3.22)
project(demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(demo src/main.cpp include/Parser_noRegex.hpp include/Parser_noRegex_fgets.hpp include/helpFunc.hpp include/parser_regex.hpp include/parser_regex_openmp.hpp include/parser_multiThread.h)
