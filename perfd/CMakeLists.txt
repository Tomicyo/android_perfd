
macro(add_mod dir)
    set(options)
    set(oneValueArgs "CATEGORY")
    set(multiValueArgs EXHEADERS)
    cmake_parse_arguments(add_mod "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach(name ${add_mod_UNPARSED_ARGUMENTS})
        list(APPEND ${dir}_SRCS ${dir}/${name})
    endforeach(name)

    source_group(${dir} FILES ${${dir}_SRCS})
    set(LIBSRC ${LIBSRC} ${${dir}_SRCS})
endmacro(add_mod)

set(PD_SRCS
    agent_service.cc
    agent_service.h
    connector.cc
    connector.h
    daemon.cc
    daemon.h
    generic_component.cc
    generic_component.h
    perfd.cc
    profiler_component.h
    profiler_service.cc
    profiler_service.h
)
source_group(daemon FILES ${PD_SRCS})

add_mod(cpu
cpu_cache.h
cpu_collector.h
cpu_profiler_component.h
cpu_service.h
cpu_usage_sampler.h
procfs_files.h
simpleperf_manager.h
thread_monitor.h
threads_sample.h
cpu_cache.cc
cpu_collector.cc
cpu_service.cc
cpu_usage_sampler.cc
procfs_files.cc
simpleperf_manager.cc
thread_monitor.cc
)

add_mod(event
event_cache.cc
event_service.cc
internal_event_service.cc
event_cache.h
event_profiler_component.h
event_service.h
internal_event_service.h
)

add_mod(graphics
graphics_cache.h
graphics_collector.h
graphics_framestats_sampler.h
graphics_profiler_component.h
graphics_service.h
graphics_cache.cc
graphics_collector.cc
graphics_framestats_sampler.cc
graphics_service.cc
)

add_mod(memory
internal_memory_service.cc
memory_cache.cc
memory_collector.cc
memory_levels_sampler.cc
memory_service.cc
internal_memory_service.h
memory_cache.h
memory_collector.h
memory_levels_sampler.h
memory_profiler_component.h
memory_service.h
)

add_mod(network
connection_sampler.cc
connectivity_sampler.cc
internal_network_service.cc
io_network_type_provider.cc
net_stats_file_reader.cc
network_cache.cc
network_collector.cc
network_service.cc
speed_converter.cc
speed_sampler.cc
connection_details.h
connection_sampler.h
connectivity_sampler.h
fake_network_type_provider.h
internal_network_service.h
io_network_type_provider.h
net_stats_file_reader.h
network_cache.h
network_collector.h
network_constants.h
network_profiler_component.h
network_sampler.h
network_service.h
network_type_provider.h
speed_converter.h
speed_sampler.h
)

set(PROTO_SRCS 
    ${PROF_PROTO_GENS_DIR}/agent_service.pb.cc
    ${PROF_PROTO_GENS_DIR}/agent_service.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/cpu.pb.cc
    ${PROF_PROTO_GENS_DIR}/cpu.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/event.pb.cc
    ${PROF_PROTO_GENS_DIR}/event.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/graphics.pb.cc
    ${PROF_PROTO_GENS_DIR}/graphics.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/internal_event.pb.cc
    ${PROF_PROTO_GENS_DIR}/internal_event.grpc.pb.cc
    
    ${PROF_PROTO_GENS_DIR}/internal_memory.pb.cc
    ${PROF_PROTO_GENS_DIR}/internal_memory.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/internal_network.pb.cc
    ${PROF_PROTO_GENS_DIR}/internal_network.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/io.pb.cc
    ${PROF_PROTO_GENS_DIR}/io.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/memory.pb.cc
    ${PROF_PROTO_GENS_DIR}/memory.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/network.pb.cc
    ${PROF_PROTO_GENS_DIR}/network.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/profiler.pb.cc
    ${PROF_PROTO_GENS_DIR}/profiler.grpc.pb.cc
    
    ${PROF_PROTO_GENS_DIR}/profiler_service.pb.cc
    ${PROF_PROTO_GENS_DIR}/profiler_service.grpc.pb.cc

    ${PROF_PROTO_GENS_DIR}/simplerperf_report.pb.cc
    ${PROF_PROTO_GENS_DIR}/simplerperf_report.grpc.pb.cc
)
set_source_files_properties(${PROTO_SRCS} PROPERTIES GENERATED TRUE)

add_executable(perfd ${LIBSRC} ${PD_SRCS} ${PROTO_SRCS})

target_link_libraries(perfd
perf_utils
${AndroPerf_SOURCE_DIR}/depend/lib/libgpr.a
${AndroPerf_SOURCE_DIR}/depend/lib/libgrpc.a
${AndroPerf_SOURCE_DIR}/depend/lib/libgrpc_unsecure.a
${AndroPerf_SOURCE_DIR}/depend/lib/libgrpc_cronet.a
grpc++
${AndroPerf_SOURCE_DIR}/depend/lib/libprotobuf.a
log
)

set_property(TARGET perfd PROPERTY CXX_STANDARD 14)
target_add_proto(perfd PROTOS
  ../protos/agent_service.proto
  ../protos/cpu.proto
  ../protos/event.proto
  ../protos/graphics.proto
  ../protos/internal_event.proto
  ../protos/internal_memory.proto
  ../protos/internal_network.proto
  ../protos/io.proto
  ../protos/memory.proto
  ../protos/network.proto
  ../protos/profiler.proto
  ../protos/profiler_service.proto
  ../protos/simplerperf_report.proto
)