cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# SET PROJECT NAME
project(pathfinding)

# SET C LANGUAGE STANDARD
set(C_STANDARD 23)
set(C_STANDARD_REQUIRED 23)

add_executable(main ${CMAKE_SOURCE_DIR}/main.c)

add_subdirectory(src)
target_link_libraries(main PRIVATE src)

# LINK SDL3 LIBRARY
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE SDL3::SDL3)