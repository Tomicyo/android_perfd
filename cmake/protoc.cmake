set(PROTOC_EXECUTABLE "protoc")
set(GRPC_CPP_PLUGIN_EXECUTABLE "G:/android_profiler_qt_lib/build/x64/grpc_cpp_plugin.exe")
set(PROF_PROTO_GENS_DIR ${AndroPerf_SOURCE_DIR}/build/proto)
file(MAKE_DIRECTORY ${PROF_PROTO_GENS_DIR})
set(PROF_PROTO_INC_DIR ${AndroPerf_SOURCE_DIR}/protos)

include_directories(${AndroPerf_SOURCE_DIR}/build)
include_directories(${AndroPerf_SOURCE_DIR}/thirdparty/grpc/third_party/protobuf/src)
include_directories(${AndroPerf_SOURCE_DIR}/thirdparty/grpc/include)

function(add_proto TARGET)
    cmake_parse_arguments(${TARGET}
        ""
        "FOLDER;OUTPUT_VAR"
        "PROTOS"
        ${ARGN}
    )
    foreach(PROTO IN LISTS ${TARGET}_PROTOS)
    	get_filename_component(ABS_PROTO ${PROTO} ABSOLUTE)
    	get_filename_component(PROTO_NAME ${PROTO} NAME_WE)
    	#add_custom_command(TARGET ${TARGET} PRE_BUILD 
		execute_process(COMMAND
			protoc
			--grpc_out=generate_mock_code=true:${PROF_PROTO_GENS_DIR}
			--cpp_out=${PROF_PROTO_GENS_DIR} 
            --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
			-I ${PROF_PROTO_INC_DIR}
			${ABS_PROTO}
        )
        
        list(APPEND PROTOSRC_LIST ${PROF_PROTO_GENS_DIR}/${PROTO_NAME}.pb.cc ${PROF_PROTO_GENS_DIR}/${PROTO_NAME}.grpc.pb.cc)
    endforeach()
    set(${${TARGET}_OUTPUT_VAR} ${PROTOSRC_LIST} PARENT_SCOPE)
endfunction()