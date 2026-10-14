cmake_minimum_required(VERSION 3.21)

project(Binz2 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# ─── Auto-detect Geode SDK ─────────────────────────────────
# Composite action không set CMAKE_MODULE_PATH, phải tự tìm.
if(DEFINED ENV{GEODE_SDK} AND EXISTS "$ENV{GEODE_SDK}/cmake/Geode.cmake")
    set(GEODE_SDK_PATH "$ENV{GEODE_SDK}")
    message(STATUS "Found SDK from GEODE_SDK env: ${GEODE_SDK_PATH}")

elseif(DEFINED ENV{CLI_PROFILE} AND EXISTS "$ENV{CLI_PROFILE}/sdk/cmake/Geode.cmake")
    set(GEODE_SDK_PATH "$ENV{CLI_PROFILE}/sdk")
    message(STATUS "Found SDK in CLI_PROFILE: ${GEODE_SDK_PATH}")

elseif(EXISTS "$ENV{USERPROFILE}/.geode/cmake/Geode.cmake")
    set(GEODE_SDK_PATH "$ENV{USERPROFILE}/.geode")
    message(STATUS "Found SDK in USERPROFILE: ${GEODE_SDK_PATH}")

else()
    message(STATUS "GEODE_SDK env = $ENV{GEODE_SDK}")
    message(STATUS "CLI_PROFILE env = $ENV{CLI_PROFILE}")
    message(STATUS "USERPROFILE = $ENV{USERPROFILE}")
    message(FATAL_ERROR "Không tìm thấy Geode.cmake ở bất kỳ path nào ở trên")
endif()

list(APPEND CMAKE_MODULE_PATH "${GEODE_SDK_PATH}/cmake")
include("${GEODE_SDK_PATH}/cmake/Geode.cmake")
# ───────────────────────────────────────────────────────────

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/ui/SolverPopup.cpp
)

setup_geode_mod(${PROJECT_NAME})