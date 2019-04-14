cmake_minimum_required(VERSION 3.12)
project(Filozofowie)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lncurses")

add_executable(Filozofowie Graphics.cpp)