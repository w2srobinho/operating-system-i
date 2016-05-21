
project(validator)


set(HEADER_FILES
    sudoku.h
)

set(SOURCE_FILES
    sudoku.cpp
)

add_library(validator
    ${SOURCE_FILES}
    ${HEADER_FILES}
)
