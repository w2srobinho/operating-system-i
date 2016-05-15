project(validator_tests CXX C)

include_directories(
    ${gtest_SOURCE_DIR}/include
    ${validator_SOURCE_DIR}
)

set(SOURCE_FILES
    sample1_unittest.cc
)

add_executable(validator_tests
    ${SOURCE_FILES}
)

target_link_libraries(validator_tests
    validator
    gtest_main
)

add_gtest(validator_tests)
