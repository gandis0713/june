cmake_minimum_required(VERSION 3.22)

project(
  synchronizer
  VERSION 0.0.1
  DESCRIPTION "External Resource Synchronizer"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_executable(synchronizer
  ${CMAKE_SOURCE_DIR}/src/main.cpp
)