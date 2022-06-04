cmake_minimum_required(VERSION 3.16)

project(Cerelatis-core-lib)

set(Project Cerelatis-core-lib)

set(CMAKE_CXX_FLAGS "-std=gnu++2a -O3")

add_compile_definitions(LOG_WITH_TIME)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(PUBLIC_INCLUDES
    ${INC_DIR}/Net/Address.h
    ${INC_DIR}/Net/Client.h
    ${INC_DIR}/Net/Port.h
    ${INC_DIR}/Net/Server.h
    ${INC_DIR}/Net/ServerType.h
)

set(PRIVATE_SOURCES
    ${SRC_DIR}/Net/Server.cpp
    ${SRC_DIR}/Net/Client.cpp
    ${SRC_DIR}/utils/ThreadPool.cpp
)

set(ALL_INCLUDES
    ${PUBLIC_INCLUDES}
    ${PRIVATE_SOURCES}
)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${INC_DIR}
    ${LIB_DIR}
    ${LIB_DIR}
)

add_subdirectory(
    lib/Logger
)
include_directories(
    ${LIB_DIR}/Logger/includes
)

add_library(${Project}
    STATIC
        ${ALL_INCLUDES}
)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

target_link_libraries(${Project}
    logger-lib
    Threads::Threads
)