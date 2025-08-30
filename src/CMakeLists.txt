add_library(src)

set(HEADERS
    # ${CMAKE_SOURCE_DIR}/src/.h
)

set(SOURCES 
    ${CMAKE_SOURCE_DIR}/src/grid.c
)

target_sources(src PRIVATE ${SOURCES} PUBLIC ${HEADERS})
target_include_directories(src PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# LINK SDL3 LIBRARY
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(src PRIVATE SDL3::SDL3)