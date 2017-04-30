cmake_minimum_required(VERSION 3.6)
project(SM3Collision)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp sm3.h sm3.c)
add_executable(SM3Collision ${SOURCE_FILES})