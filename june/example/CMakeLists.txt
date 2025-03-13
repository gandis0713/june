cmake_minimum_required(VERSION 3.22)

project(
  june-example
  VERSION 0.0.1
  DESCRIPTION "June Example"
  LANGUAGES CXX
)

add_executable(june-example
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)