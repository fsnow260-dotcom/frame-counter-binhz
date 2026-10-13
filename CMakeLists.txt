cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(NaNDLPrecision VERSION 1.0.0 LANGUAGES CXX)

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
else()
    target_sources(${PROJECT_NAME} PRIVATE src/platform/notwindows_input.cpp)
endif()

# Macro này được cung cấp bởi Geode SDK, nó sẽ tự động tìm và link mọi thứ.
setup_geode_mod(${PROJECT_NAME})