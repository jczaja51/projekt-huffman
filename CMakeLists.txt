cmake_minimum_required(VERSION 3.16)
project(projekt-aisd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(projekt-aisd
        src/main.cpp
        src/minheap.cpp
        src/huffman.cpp
        src/io.cpp
        src/huffman.h
        src/io.h
)

target_include_directories(projekt-aisd PRIVATE src)
