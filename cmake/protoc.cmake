
set(PROF_PROTO_GENS_DIR ${AndroPerf_SOURCE_DIR}/build/proto)
file(MAKE_DIRECTORY ${PROF_PROTO_GENS_DIR})
set(PROF_PROTO_INC_DIR ${AndroPerf_SOURCE_DIR}/protos)

function(target_add_proto TARGET)
    cmake_parse_arguments(${TARGET}
        ""
        "FOLDER"
        "PROTOS"
        ${ARGN}
    )
    foreach(PROTO IN LISTS ${TARGET}_PROTOS)
    	get_filename_component(ABS_PROTO ${PROTO} ABSOLUTE)
    	#add_custom_command(TARGET ${TARGET} PRE_BUILD 
		execute_process(COMMAND
			protoc
			--grpc_out=generate_mock_code=true:${PROF_PROTO_GENS_DIR}
			--cpp_out=${PROF_PROTO_GENS_DIR} 
            --plugin=protoc-gen-grpc=G:/android_profiler_qt_lib/build/x64/grpc_cpp_plugin.exe
			-I ${PROF_PROTO_INC_DIR}
			${ABS_PROTO}
		)
    endforeach()
endfunction()