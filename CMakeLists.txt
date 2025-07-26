cmake_minimum_required(VERSION 3.10)
project(MazeGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(maze.out ${SRC_FILES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(COPY static/ DESTINATION ${CMAKE_BINARY_DIR}/static)