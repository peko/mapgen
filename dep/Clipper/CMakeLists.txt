cmake_minimum_required(VERSION 3.10)
project(clipper
   VERSION 2.0
   DESCRIPTION "clipper wraper"
   LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-g -O3")
include_directories(${CURRENT_SOURCE_DIR})
add_library(clipper
   clipper.cpp
)
