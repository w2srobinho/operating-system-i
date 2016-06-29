project(core_tests CXX C)

include_directories(
    ${gtest_SOURCE_DIR}/include
    ${core_SOURCE_DIR}
)

set(SOURCE_FILES
        queue_tests.cpp
        task_tests.cpp)

add_executable(core_tests
    ${SOURCE_FILES}
)

target_link_libraries(core_tests
    core
    ${CMAKE_THREAD_LIBS_INIT}
    gtest_main
)

add_gtest(core_tests)
