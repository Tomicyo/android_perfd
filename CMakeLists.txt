cmake_minimum_required(VERSION 3.2)

# ANDROID API LEVEL NEED GREATER THAN 21
project(AndroPerf)

set(GRPC_INCLUDE_DIRS 		${AndroPerf_SOURCE_DIR}/depend/include)
set(PROTOBUF_INCLUDE_DIRS 	${AndroPerf_SOURCE_DIR}/depend/include)

include_directories(${GRPC_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories(${AndroPerf_SOURCE_DIR})

include(cmake/protoc.cmake)

include_directories(${AndroPerf_SOURCE_DIR}/build)
link_directories(${AndroPerf_SOURCE_DIR}/depend/lib)

add_subdirectory(utils)
add_subdirectory(agent)
add_subdirectory(perfa)
add_subdirectory(perfd)