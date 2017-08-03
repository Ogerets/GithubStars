
cmake_minimum_required(VERSION 2.8.9)
project(parser)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCES "src/*.cpp")
include_directories("include")

add_executable(parser ${SOURCES})

include_directories("dep/curl/include")
target_link_libraries(parser libcurl.a)

