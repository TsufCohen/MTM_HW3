cmake_minimum_required(VERSION 3.13)
project(204511414_308087683)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic-errors -Werror -g -v")

add_executable(204511414_308087683 main.cpp Complex.cpp MtmExceptions.h
               Auxilaries.h Complex.h MtmVec.h MtmMat.h MtmMatSq.h MtmMatTriag.h)