cmake_minimum_required(VERSION 3.10.0) 
project(Smpl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB_RECURSE SRCS
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*"
)

add_executable(app ${SRCS})
