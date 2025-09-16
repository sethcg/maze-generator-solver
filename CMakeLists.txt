cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# SET PROJECT NAME
project(maze-generation)

# SET C LANGUAGE STANDARD
set(C_STANDARD 17)
set(C_STANDARD_REQUIRED 17)

# LINK SDL3 LIBRARY (TO ENTIRE PROJECT)
find_package(SDL3 CONFIG REQUIRED)

# ADD CORE DIRECTORY
add_subdirectory(core)

# CREATE EXECUTABLE
add_executable(main ./main.c)

# INCLUDE CORE TO TARGET
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# LINK LIBARIES TO TARGET
target_link_libraries(main PRIVATE 
    core
    SDL3::SDL3
)