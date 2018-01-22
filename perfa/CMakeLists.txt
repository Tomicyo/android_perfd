set(PERFA_SRCS
    jvmti_helper.cc
    jvmti_helper.h
    memory/memory_tracking_env.cc
    memory/memory_tracking_env.h
    memory/stats.h
    perfa.cc
    scoped_local_ref.h
)

add_library(perfa SHARED ${PERFA_SRCS})
target_link_libraries(perfa protos)