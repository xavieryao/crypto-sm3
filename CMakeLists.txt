cmake_minimum_required(VERSION 3.6)
project(SM3Collision)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp SM3.cpp SM3.h)
add_executable(SM3Collision ${SOURCE_FILES})