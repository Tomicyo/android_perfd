
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

add_executable(perfd ${LIBSRC} ${PD_SRCS})
target_link_libraries(perfd
perf_utils
grpc
grpc++
libprotobuf
)