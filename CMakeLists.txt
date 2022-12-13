# Copyright © 2022 Zen Shawn. All rights reserved.
# Created at 20:51:10, December 12, 2022

cmake_minimum_required(VERSION 3.13)

option(BUILD_SHARED_LIBS "build shared libs" OFF)

set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "build type, Release/Debug/MinSizeRel/RelWithDebInfo")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(trajectories VERSION 0.0.0 DESCRIPTION "Instroduction about trajectories")

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra)
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(STATUS "Found ccache")
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

add_definitions(-D_USE_MATH_DEFINES)
include_directories(Src)
add_subdirectory(Src)
add_subdirectory(Examples)