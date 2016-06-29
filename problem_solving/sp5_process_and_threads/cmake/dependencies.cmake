if(_DEPENDENCIES_INCLUDED)
    return()
endif()
set(_DEPENDENCIES_INCLUDED TRUE)


set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
if (NOT EXISTS ${THIRD_PARTY_DIR})
    file(MAKE_DIRECTORY ${THIRD_PARTY_DIR})
endif()

include(gtest)

get_gtest(${THIRD_PARTY_DIR}/gtest)
