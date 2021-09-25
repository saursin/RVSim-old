cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RVSim VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(rvsim ${SRC_FILES})

target_include_directories(rvsim PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/include)
target_include_directories(rvsim PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/include/elfio)
target_include_directories(rvsim PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/include/cxxopts)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-DRV_XLEN_32")