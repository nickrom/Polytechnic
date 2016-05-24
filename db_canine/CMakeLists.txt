cmake_minimum_required(VERSION 3.2)
project(db_canine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(db_canine ${SOURCE_FILES})