cmake_minimum_required(VERSION 3.11)

project(mapgen)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)
set(CMAKE_C_FLAGS "-g -O0 -std=c11 -fms-extensions")

add_definitions(-DNANOVG_GL2 -DNANOVG_GLEW)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/clipper
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/nanovg/src
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/delaunay 
)

add_library(nanovg STATIC 
    dep/nanovg/src/nanovg.c
)

add_library(clipper STATIC 
    src/clipper/clipper.cpp
    src/clipper/clipper_c.cpp
)

add_library(visilibity STATIC 
    src/visilibity/visilibity.cpp
    src/visilibity/visilibity_c.cpp
)

add_library(delaunay STATIC
    dep/delaunay/delaunay.c
)

# add_executable(clipper_test
#     /dep/Clipper/clipper_test.cpp
# )
# target_link_libraries(clipper_test clipper)
# add_executable(clipper_c_test
#    Clipper/clipper_c_test.c
# )
# target_link_libraries(clipper_c_test clipper)
 
add_executable(mapgen
    src/mapgen.c
    src/engine.c
    src/utils.c 
    src/mgLevel.c
    src/mgRoom.c
)

target_link_libraries(mapgen 
    nanovg 
    ${OPENGL_gl_LIBRARY} 
    ${GLEW_LIBRARIES} 
    GL 
    glfw 
    m 
    chipmunk
    clipper
    visilibity
	delaunay
)

add_custom_target(
    run
    COMMAND mapgen
    DEPENDS mapgen
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
