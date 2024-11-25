cmake_minimum_required(VERSION 3.10)

# Set the project name
project(
    memory
    LANGUAGES C
)

include_directories(include)

# Flags for compiling
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_RELEASE "-O0 -Wall -Wextra -Wpedantic -Werror -Wunused-parameter -Wmissing-prototypes -Wstrict-prototypes")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage -Wall -Wextra -Wpedantic -Werror -Wunused-parameter -Wmissing-prototypes -Wstrict-prototypes")

# Add executable for the main project
add_executable(${PROJECT_NAME} src/main.c src/memory.c)