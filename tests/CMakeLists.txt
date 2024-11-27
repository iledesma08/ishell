# Verify cmake version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Create project for tests
project(tests LANGUAGES C)

# Enable testing
include(CTest)

# Find the unity package
find_package(unity REQUIRED)

# Include directories for tests
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add executable for tests
add_executable(tests 
    ${CMAKE_SOURCE_DIR}/tests/test_mem.c
    ${CMAKE_SOURCE_DIR}/src/memory.c 
    ${CMAKE_SOURCE_DIR}/src/malloc.c 
    ${CMAKE_SOURCE_DIR}/src/free.c 
    ${CMAKE_SOURCE_DIR}/src/realloc.c 
    ${CMAKE_SOURCE_DIR}/src/calloc.c 
    ${CMAKE_SOURCE_DIR}/src/mem_logging.c 
    ${CMAKE_SOURCE_DIR}/src/mem_metrics.c 
    ${CMAKE_SOURCE_DIR}/tests/memory_test.c 
    ${CMAKE_SOURCE_DIR}/tests/heap_test.c
) 

# Specify the output directory for the tests binary within build/tests
set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)

# Link libraries for tests
target_link_libraries(tests PRIVATE unity::unity)

# Add test command
add_test(NAME imemTests COMMAND tests)

# Add coverage flags unconditionally for the tests binary
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")