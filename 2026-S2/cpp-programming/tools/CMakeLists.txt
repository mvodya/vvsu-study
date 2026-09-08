cmake_minimum_required(VERSION 3.24)

project(cpp_course_tools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED CPP_COURSE_LABS_DIR)
  set(CPP_COURSE_LABS_DIR "${CMAKE_SOURCE_DIR}/../labs")
endif()

add_subdirectory(generators)
add_subdirectory(md-to-pdf)
