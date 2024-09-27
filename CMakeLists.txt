cmake_minimum_required(VERSION 3.25)

project(tuchuang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -fmodules-ts -fcoroutines")

include_directories(${PROJECT_SOURCE_DIR/include} ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)