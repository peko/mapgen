cmake_minimum_required(VERSION 3.11)

project(mapgen)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)
set(CMAKE_C_FLAGS "-g -O0")

add_definitions(-DNANOVG_GL2 -DNANOVG_GLEW)
include_directories(nanovg/src/)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})

add_library(nanovg STATIC 
    nanovg/src/nanovg.c
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
    ${CMAKE_CURRENT_SOURCE_DIR}/Clipper)

add_library(clipper STATIC 
    Clipper/clipper.cpp
    Clipper/clipper_c.cpp
)
add_executable(clipper_test
    Clipper/clipper_test.cpp
)
target_link_libraries(clipper_test clipper)

add_executable(clipper_c_test
    Clipper/clipper_c_test.c
)
target_link_libraries(clipper_c_test clipper)

 
add_executable(mapgen
    src/mapgen.c
    src/engine.c
    src/utils.c 
    src/level.c
    src/room.c
    src/tunnel.c
)

target_link_libraries(mapgen 
    nanovg 
    ${OPENGL_gl_LIBRARY} 
    ${GLEW_LIBRARIES} 
    GL 
    glfw 
    m 
    chipmunk
    clipper)
