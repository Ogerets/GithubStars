
cmake_minimum_required(VERSION 2.8.9)
project(parser)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCES "src/*.*pp")

add_executable(parser ${SOURCES})

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(parser ${CURL_LIBRARIES})