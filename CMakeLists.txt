cmake_minimum_required(VERSION 3.6)
project(Ex2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c)
add_executable(Ex2 ${SOURCE_FILES})