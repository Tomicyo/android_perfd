
set(AGENT_SRCS
    agent.cc
    agent.h
    agent_component.h
    memory_component.cc
    memory_component.h
    support/event_manager.cc
    support/event_manager.h
    support/event_passthrough.cc
    support/http_tracker.cc
    support/jni_wrappers.h
    support/memory_stats_logger.cc
    support/memory_stats_logger.h
    support/profiler_service.cc
)

add_library(supportjni SHARED ${AGENT_SRCS})