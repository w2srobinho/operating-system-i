
project(core)


set(HEADER_FILES
    Queue.h
    Task.h
)

set(SOURCE_FILES
    Queue.cc
    Task.cc
)

add_library(core
    ${SOURCE_FILES}
    ${HEADER_FILES}
)
