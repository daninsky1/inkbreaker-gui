cmake_minimum_required(VERSION 3.26)

project(InkBreakerGUI VERSION 0.1.0 LANGUAGES C CXX)

# Add this line near the top of your CMakeLists.txt, after 'project()'
# This sets the CMAKE_MSVC_RUNTIME_LIBRARY variable to statically link the C++ runtime.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:>" CACHE STRING "MSVC runtime library setting")
set_property(CACHE CMAKE_MSVC_RUNTIME_LIBRARY PROPERTY STRINGS "MultiThreaded" "MultiThreadedDebug" "MultiThreadedDLL" "MultiThreadedDebugDLL")

# You can also use this, which is a simpler approach that works for most cases
# set(CMAKE_CXX_FLAGS "/MT")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

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

set(SOURCES
        src/examples.h
        src/widget.cpp
        src/window.cpp
        src/window.h
        src/container.cpp
        src/container.h
        src/flex_container.cpp
        src/flex_container.h
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
        src/calculator.cpp
        src/calculator.h
        src/overlay.cpp
        src/overlay.h
        src/render_strategy.cpp
        src/render_strategy.h
        src/render_types.h
        src/graphics/geometry.h
)

add_executable(inkbreaker_gui_test src/main.cpp ${SOURCES})
target_include_directories(inkbreaker_gui_test PRIVATE src)
# Add this flag to your target's compile options
target_compile_options(inkbreaker_gui_test PRIVATE -g -fsanitize=address -fno-omit-frame-pointer)
target_link_options(inkbreaker_gui_test PRIVATE -fsanitize=address)

add_executable(workbench tests/workbench.cpp ${SOURCES})
target_include_directories(workbench PRIVATE src)

add_dependencies(inkbreaker_gui_test copy_resources)

add_library(tinyexpr STATIC vendor/tinyexpr/tinyexpr.c)
add_library(tinyexpr::tinyexpr ALIAS tinyexpr)
target_include_directories(tinyexpr PUBLIC vendor/tinyexpr)
if (UNIX)
        target_link_libraries(tinyexpr PUBLIC m)
endif()

include(FetchContent)

# SDL 3
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.18
)
FetchContent_MakeAvailable(SDL3)
target_link_libraries(inkbreaker_gui_test PRIVATE SDL3::SDL3-static)
target_link_libraries(inkbreaker_gui_test PRIVATE tinyexpr::tinyexpr)

# Boost
set(Boost_INCLUDE_DIRS "C:/Users/daniel/dev/boost_1_88_0")
target_include_directories(inkbreaker_gui_test PRIVATE ${Boost_INCLUDE_DIRS})

# Asmjit required by Blend2d
set(ASMJIT_DIR "${CMAKE_SOURCE_DIR}/vendor/asmjit"
        CACHE PATH "Location of 'blend2d'")

# Blend2d
set(BLEND2D_DIR "${CMAKE_SOURCE_DIR}/vendor/blend2d"
        CACHE PATH "Location of 'blend2d'")
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

target_link_libraries(inkbreaker_gui_test PRIVATE blend2d::blend2d)

# Executável de testes
target_link_libraries(workbench PRIVATE SDL3::SDL3)
target_include_directories(workbench PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(workbench PRIVATE tinyexpr::tinyexpr)
target_link_libraries(workbench PRIVATE blend2d::blend2d)
