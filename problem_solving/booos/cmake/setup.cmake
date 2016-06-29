if(_SETUP_INCLUDED)
    return()
endif()
set(_SETUP_INCLUDED TRUE)

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

include(dependencies)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    # Warning checks for Clang or GNU compilers
    set(warnings "-Wall -Wextra -Werror -Wno-error=unused-parameter")

    # add c++11 support
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} -std=c++11")
endif()
