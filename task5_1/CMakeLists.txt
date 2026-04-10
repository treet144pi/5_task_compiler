cmake_minimum_required(VERSION 3.10)

project(LoopsAnalyser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(loop_analyzer src/main.cpp
                             src/graph.cpp
                             src/loops.cpp
)
target_include_directories(loop_analyzer PRIVATE include)
