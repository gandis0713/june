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
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api_context.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api_context.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api_memory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_api_memory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_fence.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/vulkan/vulkan_fence.h

  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_context.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_context.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_api.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_api.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_shared_memory.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_shared_memory.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_buffer.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_buffer.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_texture.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_texture.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_fence.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/native/gles/gles_fence.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/instance.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/instance.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/api_memory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/api_memory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/shared_memory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/shared_memory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/native/fence.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/fence.h
  ${CMAKE_CURRENT_SOURCE_DIR}/memory/raw_memory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/memory/raw_memory.h
)

if(ANDROID)
  list(APPEND JUNE_NATIVE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/ahardwarebuffer_memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/ahardwarebuffer_memory.h
  )
endif()

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
