cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# SET PROJECT NAME
project(maze-generation)

# SET C LANGUAGE STANDARD
set(C_STANDARD 17)
set(C_STANDARD_REQUIRED 17)

# LINK SDL3 LIBRARY (TO ENTIRE PROJECT)
find_package(SDL3 CONFIG REQUIRED)
link_libraries(SDL3::SDL3)

# ADD COMMON DIRECTORY
add_subdirectory(common)

# CREATE EXECUTABLE
add_executable(main ./main.c)

target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# LINK COMMON TO MAIN FILE
target_link_libraries(main common)