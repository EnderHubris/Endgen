cmake_minimum_required(VERSION 2.8)
project(endgen)

# enable ctest
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wno-reorder -Wunused-variable -Wunused-parameter -Wnarrowing -Werror=narrowing -Wno-error=all")

# find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_TTF REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the source directory with its own CMakeLists.txt
add_subdirectory(engine/src)