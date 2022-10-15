cmake_minimum_required(VERSION 3.22)
project(demo)

set(CMAKE_CXX_STANDARD 17 -pthread -fopenmp -ftree-vectorize)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/third_library/rejit/include)
link_directories(${CMAKE_SOURCE_DIR}/third_library/rejit/lib)

add_executable(demo
        src/main.cpp
        src/parser_regex.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(demo PUBLIC OpenMP::OpenMP_CXX rejit)
else()
    target_link_libraries(demo PUBLIC rejit)
endif()