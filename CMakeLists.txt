cmake_minimum_required(VERSION 3.2)

# ANDROID API LEVEL NEED GREATER THAN 21
project(AndroPerf)
include_directories(${AndroPerf_SOURCE_DIR})
include(cmake/protoc.cmake)

add_subdirectory(thirdparty/grpc)

add_subdirectory(utils)
add_subdirectory(agent)
add_subdirectory(perfa)
add_subdirectory(perfd)