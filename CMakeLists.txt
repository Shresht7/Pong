cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(Pong C)

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)

# Add include directory
include_directories(src)

# Add the executable and specify its source file
add_executable(pong src/main.c src/game.c src/terminal.c)
