cmake_minimum_required(VERSION 3.2)

project(AndroPerf)

include_directories(${GRPC_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})

add_subdirectory(utils)
add_subdirectory(perfa)
add_subdirectory(perfd)