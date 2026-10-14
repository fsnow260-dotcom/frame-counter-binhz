cmake_minimum_required(VERSION 3.21)

project(Binz2 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# ─── Load Geode SDK ────────────────────────────────────────
set(GEODE_SDK_PATH "$ENV{GEODE_SDK}")
if(NOT EXISTS "${GEODE_SDK_PATH}/cmake/GeodeFile.cmake")
    message(FATAL_ERROR "GeodeFile.cmake not found in ${GEODE_SDK_PATH}/cmake/")
endif()

list(APPEND CMAKE_MODULE_PATH "${GEODE_SDK_PATH}/cmake")
include("${GEODE_SDK_PATH}/cmake/GeodeFile.cmake")
# ───────────────────────────────────────────────────────────

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/ui/SolverPopup.cpp
)

setup_geode_mod(${PROJECT_NAME})