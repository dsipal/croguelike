# Set the cmake minimum version
cmake_minimum_required(VERSION 3.26)

# Name of the CMake project, description, and what languages it uses
project(LibtcodTutorials
DESCRIPTION "Modern C++ libtcod tutorials"
LANGUAGES CXX
)

# Use C++20 standard for compiling
set(CMAKE_CXX_STANDARD 20)

find_package(libtcod CONFIG REQUIRED)

add_executable(tutorial
main.cpp
)

target_link_libraries(tutorial
PRIVATE libtcod::libtcod
)

# Copy the terminal.png file needed to properly render tiles
configure_file("${CMAKE_SOURCE_DIR}/terminal.png" "${CMAKE_BINARY_DIR}" COPYONLY)

