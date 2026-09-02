cmake_minimum_required(VERSION 3.20)

project(NanoDB CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(NanoDB ${SOURCES})

# Need to be removed
target_include_directories(NanoDB PRIVATE 
    src
    src/storage
    src/catalog
    src/types
    src/common
    src/parser
    src/executor
)