cmake_minimum_required(VERSION 4.1.0)

project(Citrus LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(source)

option(CITRUS_BUILD_EXAMPLES "Build examples" on)
if (CITRUS_BUILD_SAMPLES)
  add_subdirectory(examples)
endif()
