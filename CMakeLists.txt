cmake_minimum_required(VERSION 3.15)

project(tuchuang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -std=c++20")

include_directories(${PROJECT_SOURCE_DIR/include})

add_subdirectory(${PROJECT_SOURCE_DIR}/src/conf)