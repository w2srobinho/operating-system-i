cmake_minimum_required(VERSION 3.4)
project(sps C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Threads REQUIRED)

set(SOURCE_FILES sp1_sudoku_validator/main.cpp)
add_executable(sps ${SOURCE_FILES})
target_link_libraries(sps ${CMAKE_THREAD_LIBS_INIT})