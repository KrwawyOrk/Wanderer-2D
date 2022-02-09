cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(wanderer2d)
include(Wanderer2D)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

add_compile_options(-Wall -Werror -Wimplicit-fallthrough=0 -pipe -fvisibility=hidden)

set(CMAKE_CXX_FLAGS_PERFORMANCE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fno-strict-aliasing)
endif()
