cmake_minimum_required(VERSION 3.20)

project(NanoDB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(SOURCES
    # Catalog
    src/catalog/Column.cpp
    src/catalog/Column.h
    src/catalog/Database.cpp
    src/catalog/Table.cpp
    src/catalog/Table.h

    # Common
    src/common/DatabaseException.cpp
    src/common/Utils.h

    # Executor
    src/executor/Executor.cpp

    # Parser
    src/parser/AST.h
    src/parser/Lexer.cpp
    src/parser/Lexer.h
    src/parser/Parser.cpp
    src/parser/Parser.h
    src/parser/Token.h

    # Storage
    src/storage/BufferManager.cpp
    src/storage/BufferManager.h
    src/storage/DiskManager.cpp
    src/storage/DiskManager.h
    src/storage/Page.cpp
    src/storage/Page.h

    # Types
    src/types/DataType.cpp
    src/types/DataType.h
    src/types/DataValue.cpp
    src/types/DataValue.h
    src/types/Tuple.cpp
    src/types/Tuple.h

    # Public headers
    include/nanodb/Database.h
    include/nanodb/DatabaseException.h
    include/nanodb/Executor.h
    include/nanodb/NanoDB.h
)


add_library(NanoDB_Lib STATIC ${SOURCES})

target_include_directories(NanoDB_Lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nanodb
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/catalog
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
        ${CMAKE_CURRENT_SOURCE_DIR}/src/storage
        ${CMAKE_CURRENT_SOURCE_DIR}/src/types
)

add_executable(NanoDB
    src/main.cpp
)

target_link_libraries(NanoDB
    PRIVATE NanoDB_Lib
)
