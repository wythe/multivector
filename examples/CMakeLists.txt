cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(mvcli mvcli.cpp)
