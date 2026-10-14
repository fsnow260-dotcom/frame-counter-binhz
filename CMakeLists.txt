cmake_minimum_required(VERSION 3.21)

project(Binz2 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# ─── DEBUG ─────────────────────────────────────────────────
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mod.json")
    message(STATUS "mod.json EXISTS")
else()
    message(FATAL_ERROR "mod.json NOT FOUND in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/mod.json" MODJSON_CONTENT)
message(STATUS "=== mod.json content as CMake sees it ===")
message(STATUS "BEGIN")
message(STATUS "${MODJSON_CONTENT}")
message(STATUS "END")
# ───────────────────────────────────────────────────────────

# ─── Load SDK ──────────────────────────────────────────────
set(GEODE_SDK_PATH "$ENV{GEODE_SDK}")
list(APPEND CMAKE_MODULE_PATH "${GEODE_SDK_PATH}/cmake")
include("${GEODE_SDK_PATH}/cmake/GeodeFile.cmake")
# ───────────────────────────────────────────────────────────

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/ui/SolverPopup.cpp
)

setup_geode_mod(${PROJECT_NAME})