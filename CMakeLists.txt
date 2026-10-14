cmake_minimum_required(VERSION 3.21)

project(Binz2 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# ─── Locate Geode SDK ──────────────────────────────────────
set(GEODE_SDK_PATH "$ENV{GEODE_SDK}")

if(NOT EXISTS "${GEODE_SDK_PATH}")
    message(FATAL_ERROR "GEODE_SDK env is empty or path doesn't exist: '${GEODE_SDK_PATH}'")
endif()

message(STATUS "GEODE_SDK_PATH = ${GEODE_SDK_PATH}")

# List everything in SDK so we can see its structure
file(GLOB SDK_ROOT_ENTRIES "${GEODE_SDK_PATH}/*")
message(STATUS "=== SDK root entries ===")
foreach(entry ${SDK_ROOT_ENTRIES})
    message(STATUS "  ${entry}")
endforeach()

# Find the main .cmake entry point (usually Geode.cmake, GeodeSDK.cmake, etc.)
file(GLOB_RECURSE ALL_CMAKE_FILES "${GEODE_SDK_PATH}/*.cmake")
message(STATUS "=== All .cmake files in SDK ===")
foreach(f ${ALL_CMAKE_FILES})
    message(STATUS "  ${f}")
endforeach()

# Find Geode.cmake specifically
set(GEODE_CMAKE "")
foreach(f ${ALL_CMAKE_FILES})
    get_filename_component(fname "${f}" NAME)
    if(fname STREQUAL "Geode.cmake")
        set(GEODE_CMAKE "${f}")
        break()
    endif()
endforeach()

if(GEODE_CMAKE STREQUAL "")
    message(FATAL_ERROR "Could not find Geode.cmake anywhere in ${GEODE_SDK_PATH}")
endif()

get_filename_component(GEODE_CMAKE_DIR "${GEODE_CMAKE}" DIRECTORY)
message(STATUS "Loading: ${GEODE_CMAKE}")

list(APPEND CMAKE_MODULE_PATH "${GEODE_CMAKE_DIR}")
include("${GEODE_CMAKE}")
# ───────────────────────────────────────────────────────────

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/ui/SolverPopup.cpp
)

setup_geode_mod(${PROJECT_NAME})