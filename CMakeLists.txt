cmake_minimum_required(VERSION 3.16)

project(cpptimer)

include_directories(./)

add_executable(test example/test.cpp)
target_link_libraries(test pthread)