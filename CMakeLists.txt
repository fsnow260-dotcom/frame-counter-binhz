cmake_minimum_required(VERSION 3.21)

project(Frame-counter-binz VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_library(${Frame-counter-binz} SHARED
    src/main.cpp
)

setup_geode_mod(${Frame-counter-binz})