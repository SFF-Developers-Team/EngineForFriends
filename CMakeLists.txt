cmake_minimum_required(VERSION 3.21)
project(EFF VERSION 0.1.0 LANGUAGES CXX)

option(EFF_BUILD_EXAMPLES "Build EFF examples" ${PROJECT_IS_TOP_LEVEL})

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
    NAME raylib
    GITHUB_REPOSITORY raysan5/raylib
    GIT_TAG 6.0
    OPTIONS
        "BUILD_EXAMPLES OFF"
)

CPMAddPackage(
    NAME raylib-cpp
    GITHUB_REPOSITORY RobLoach/raylib-cpp
    GIT_TAG v6.0.3
    OPTIONS
        "BUILD_EXAMPLES OFF"
)

add_library(EFF STATIC
    src/Game.cpp
)
add_library(EFF::EFF ALIAS EFF)

target_compile_features(EFF PUBLIC cxx_std_20)
target_include_directories(EFF PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${raylib_SOURCE_DIR}/src
    ${raylib-cpp_SOURCE_DIR}/include
)
target_link_libraries(EFF PUBLIC raylib)

if(EFF_BUILD_EXAMPLES)
    add_subdirectory(examples/basic)
endif()
