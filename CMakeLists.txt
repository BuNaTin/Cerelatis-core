cmake_minimum_required(VERSION 3.16)

project(Cerelatis-core-lib)

set(Project Cerelatis-core-lib)

set(CMAKE_CXX_FLAGS "-std=gnu++2a -O3")

add_compile_definitions(LOG_WITH_TIME)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(PUBLIC_INCLUDES
    ${INC_DIR}/Hello.h
)

set(PRIVATE_SOURCES
    ${SRC_DIR}/Hello.cpp
)

set(ALL_INCLUDES
    ${PUBLIC_INCLUDES}
    ${PRIVATE_SOURCES}
)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${INC_DIR}
    ${LIB_DIR}
)

add_library(Cerelatis-core-lib
    STATIC
        ${ALL_INCLUDES}
)