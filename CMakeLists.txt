cmake_minimum_required(VERSION 3.10)
project(maze_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Crow is a header-only library, so we just need to include its directory.
include_directories(/usr/local/crow/include)

# Your project's header files.
include_directories(include)

# Define the executable.
add_executable(maze_game
    src/main.cpp
    src/maze.cpp
)

# Link to the libraries. We rely on apt-get installing these.
target_link_libraries(maze_game
    boost_system
    boost_filesystem
    ssl
    crypto
)

# Set the output directory for the executable.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# file(COPY static/ DESTINATION ${CMAKE_BINARY_DIR}/static)