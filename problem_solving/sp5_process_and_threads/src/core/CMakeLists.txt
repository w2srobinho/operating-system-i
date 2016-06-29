
project(core)


set(HEADER_FILES
    Process.h
    Queue.h
    Scheduler.h
    Scheduling_Queue.h
    Thread.h
    Traits.h
)

set(SOURCE_FILES
    Process.cpp
    Queue.cpp
    Scheduler.cpp
    Scheduling_Queue.cpp
    Thread.cpp
    TimerMediator.cpp
)

add_library(core
    ${SOURCE_FILES}
    ${HEADER_FILES}
)
