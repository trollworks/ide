cmake_minimum_required(VERSION 3.22.0)
project(trollworks CXX)

include(cmake/assert.cmake)
include(cmake/config.cmake)

add_subdirectory(vendor)
add_subdirectory(sources)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist/debug")
else()
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist/release")
endif()
