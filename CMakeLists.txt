cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# SET PROJECT NAME
project(template C)

# SET C LANGUAGE STANDARD
set(C_STANDARD 23)
set(C_STANDARD_REQUIRED 23)

# SET PROJECT MAIN FILE LOCATION
set(MAIN_FILE ./src/main.c)

# ADD EXECUTABLE OUPUT FILE
add_executable(main ${MAIN_FILE})

# LINK SDL3
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE SDL3::SDL3)