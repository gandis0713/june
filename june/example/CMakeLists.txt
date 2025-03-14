cmake_minimum_required(VERSION 3.22)

project(
  june-example
  VERSION 0.0.1
  DESCRIPTION "June Example"
  LANGUAGES CXX
)

find_package(spdlog REQUIRED)

add_executable(june-example
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/base/june_api.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/base/june_api.h
)

target_include_directories(june-example
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(june-example
  PRIVATE
  june::june
  spdlog::spdlog_header_only
)