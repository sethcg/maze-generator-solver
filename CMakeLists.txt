cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# SET PROJECT NAME
project(pathfinding)

# SET C LANGUAGE STANDARD
set(C_STANDARD 23)
set(C_STANDARD_REQUIRED 23)

# CREATE EXECUTABLE
add_executable(main ${CMAKE_SOURCE_DIR}/main.c)

# INCLUDE LOCAL SOURCE/HEADERS FILES
target_include_directories(main
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/)

# LINK SDL3 LIBRARY
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE SDL3::SDL3)