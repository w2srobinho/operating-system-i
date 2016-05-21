project(sudoku-validator CXX C)
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(build_tests "Build all tests Project." ON)

include(setup)

# sources
add_subdirectory(src/validator)

include_directories(
        ${validator_SOURCE_DIR})
add_executable(sudoku main.cpp)
target_link_libraries(sudoku
        ${CMAKE_THREAD_LIBS_INIT}
        validator)

# third_party
if(build_tests)
    # googletest and googlemock dependencies
    add_subdirectory(${GTEST_ROOT}/googlemock)
    enable_testing()

    # tests
    add_subdirectory(tests)
endif()
