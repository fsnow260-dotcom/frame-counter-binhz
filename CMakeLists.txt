cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(NaNDLPrecision VERSION 1.0.0 LANGUAGES CXX)

# ─── Load Geode SDK CMake module ──────────────────────────
# Action build-geode-mod set biến môi trường GEODE_SDK,
# nhưng CMake không tự động biết file macro ở đâu.
if(DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK_PATH "$ENV{GEODE_SDK}")
elseif(DEFINED GEODE_SDK)
    set(GEODE_SDK_PATH "${GEODE_SDK}")
else()
    message(FATAL_ERROR "GEODE_SDK chưa được set. Kiểm tra workflow build.yml.")
endif()

list(APPEND CMAKE_MODULE_PATH "${GEODE_SDK_PATH}/cmake")
include("${GEODE_SDK_PATH}/cmake/Geode.cmake" OPTIONAL RESULT_VARIABLE _geode_included)

if(NOT _geode_included)
    message(FATAL_ERROR "Không tìm thấy Geode.cmake tại ${GEODE_SDK_PATH}/cmake/")
endif()
# ──────────────────────────────────────────────────────────

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/nandl_solver.cpp
    src/input_hook.cpp
    src/replay_parser.cpp
    src/audio_engine.cpp
    src/export_engine.cpp
    src/ui/hud_overlay.cpp
    src/ui/SolverPopup.cpp
    src/hooks/PauseLayerHook.cpp
)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/platform/windows_input.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    target_sources(${PROJECT_NAME} PRIVATE
        src/platform/android_input.cpp
        src/platform/notwindows_input.cpp
    )
else()
    target_sources(${PROJECT_NAME} PRIVATE src/platform/notwindows_input.cpp)
endif()

setup_geode_mod(${PROJECT_NAME})