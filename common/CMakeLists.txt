# GET ALL SOURCE FILES
file(GLOB SOURCES "src/*.c")

# ADD COMMON LIBRARY
add_library(common STATIC ${SOURCES})

# INCLUDE HEADER FILES
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)