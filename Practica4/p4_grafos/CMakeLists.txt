cmake_minimum_required(VERSION 4.2)
project(p4_grafos)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(p4_grafos
        graph_coloring.cpp
        graph_coloring.h
        main.cpp)
