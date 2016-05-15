
project(validator)


set(HEADER_FILES
    sample1.h
)

set(SOURCE_FILES
    sample1.cc
)

add_library(validator
    ${SOURCE_FILES} 
    ${HEADER_FILES}
)
