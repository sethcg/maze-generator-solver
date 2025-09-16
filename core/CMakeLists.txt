# GET ALL SOURCE FILES
file(GLOB SOURCES "src/*.c")

# ADD CORE LIBRARY
add_library(core STATIC ${SOURCES})

# INCLUDE HEADER FILES
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# INCLUDE SDL3 IN CORE
target_link_libraries(core PUBLIC SDL3::SDL3)