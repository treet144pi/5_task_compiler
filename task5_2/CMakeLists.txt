cmake_minimum_required(VERSION 3.10)

project(PhiPlacement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(phi_place src/main.cpp
                         src/graph.cpp
                         src/ssa.cpp
)

target_include_directories(phi_place PRIVATE include)
