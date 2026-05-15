cmake_minimum_required(VERSION 3.10)

project(WordleSolver)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(wordle

    src/main.cpp
    src/entropy.cpp
    src/solver.cpp
    src/loader.cpp
    src/encode.cpp
)