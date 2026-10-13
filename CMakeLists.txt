cmake_minimum_required(VERSION 3.21)

project(Binz2 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/ui/SolverPopup.cpp
)

setup_geode_mod(${PROJECT_NAME})