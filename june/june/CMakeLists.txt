cmake_minimum_required(VERSION 3.22)

project(
  june
  VERSION 0.0.1
  DESCRIPTION "June Implementation"
  LANGUAGES CXX
)

find_package(VulkanHeaders CONFIG)
find_package(spdlog REQUIRED)

set(JUNE_COMMON_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/common/dylib.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common/dylib.h
)

set(JUNE_NATIVE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_context.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_context.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_buffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_buffer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_texture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_texture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/instance.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/instance.h
)

set(JUNE_SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/june.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/proc_table.cpp
)

add_library(june
  SHARED # or SHARED
  ${JUNE_NATIVE_FILES}
  ${JUNE_COMMON_FILES}
  ${JUNE_SRC_FILES}
)
add_library(june::june ALIAS june)

target_include_directories(june
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

target_link_libraries(june
  PRIVATE
  Vulkan::Headers
  spdlog::spdlog_header_only
)
