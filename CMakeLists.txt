cmake_minimum_required(VERSION 3.23)
project(fp
        VERSION 1.0.0.6
        DESCRIPTION "File Property utility"
        LANGUAGES CXX)

set(PRJ_INC_DIR ${CMAKE_SOURCE_DIR}/inc)
set(PRJ_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

configure_file(version.h.in ${PRJ_INC_DIR}/version.h)

set(CMAKE_CXX_STANDARD 20)

add_executable( fp
                ${PRJ_SRC_DIR}/main.cpp
                ${PRJ_SRC_DIR}/crc32.cpp
                ${PRJ_SRC_DIR}/csvfile.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PRJ_INC_DIR})

