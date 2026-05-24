cmake_minimum_required(VERSION 3.26)

project(InkBreakerGUI VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:>" CACHE STRING "MSVC runtime library setting")
set_property(CACHE CMAKE_MSVC_RUNTIME_LIBRARY PROPERTY STRINGS "MultiThreaded" "MultiThreadedDebug" "MultiThreadedDLL" "MultiThreadedDebugDLL")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_DIR}
        ${RESOURCE_OUTPUT_DIR}
        COMMENT "Copying resources to build folder"
)

include(FetchContent)

# SDL 3
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.18
)
FetchContent_MakeAvailable(SDL3)

# Boost
set(Boost_INCLUDE_DIRS "C:/Users/daniel/dev/boost_1_88_0")

# Asmjit required by Blend2d
set(ASMJIT_DIR "${CMAKE_SOURCE_DIR}/vendor/asmjit"
        CACHE PATH "Location of 'blend2d'")

# Blend2d
set(BLEND2D_DIR "${CMAKE_SOURCE_DIR}/vendor/blend2d"
        CACHE PATH "Location of 'blend2d'")
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

add_library(tinyexpr STATIC vendor/tinyexpr/tinyexpr.c)
add_library(tinyexpr::tinyexpr ALIAS tinyexpr)
target_include_directories(tinyexpr PUBLIC vendor/tinyexpr)
if (UNIX)
        target_link_libraries(tinyexpr PUBLIC m)
endif()

set(INKBREAKER_GUI_SOURCES
        src/framework.cpp
        src/framework.h
        src/widget.cpp
        src/window.cpp
        src/window.h
        src/container.cpp
        src/container.h
        src/center.cpp
        src/center.h
        src/align.cpp
        src/align.h
        src/padding.cpp
        src/padding.h
        src/constrained_box.cpp
        src/constrained_box.h
        src/unconstrained_box.cpp
        src/unconstrained_box.h
        src/text.cpp
        src/text.h
        src/button.cpp
        src/button.h
        src/grid.cpp
        src/grid.h
        src/overlay.cpp
        src/overlay.h
        src/render_types.h
        src/graphics/geometry.h
)

add_library(inkbreaker_gui STATIC ${INKBREAKER_GUI_SOURCES})
add_library(inkbreaker_gui::inkbreaker_gui ALIAS inkbreaker_gui)
target_include_directories(inkbreaker_gui
        PUBLIC
        src
        ${Boost_INCLUDE_DIRS}
)
target_link_libraries(inkbreaker_gui
        PUBLIC
        SDL3::SDL3-static
        blend2d::blend2d
)

function(add_inkbreaker_sample target)
        add_executable(${target} ${ARGN})
        target_include_directories(${target} PRIVATE samples/${target})
        target_link_libraries(${target} PRIVATE inkbreaker_gui::inkbreaker_gui)
        add_dependencies(${target} copy_resources)
endfunction()

add_inkbreaker_sample(calculator
        samples/calculator/main.cpp
        samples/calculator/calculator.cpp
        samples/calculator/calculator.h
)
target_link_libraries(calculator PRIVATE tinyexpr::tinyexpr)

add_inkbreaker_sample(example
        samples/example/main.cpp
        samples/example/examples.h
)

add_inkbreaker_sample(workbench
        samples/workbench/main.cpp
)
