load("//tools/base/bazel:android.bzl", "ANDROID_COPTS", "ANDROID_LINKOPTS")
load("//tools/base/bazel:android.bzl", "jni_library")

cc_binary(
    name = "libsupportjni.so",
    copts = ANDROID_COPTS,
    linkopts = ANDROID_LINKOPTS,
    linkshared = 1,
    visibility = ["//visibility:public"],
    deps = [
        ":agent",
    ],
)

cc_library(
    name = "agent",
    srcs = [
        "agent.cc",
        "agent.h",
        "agent_component.h",
        "memory_component.cc",
        "memory_component.h",
        "support/event_manager.cc",
        "support/event_manager.h",
        "support/event_passthrough.cc",
        "support/http_tracker.cc",
        "support/jni_wrappers.h",
        "support/memory_stats_logger.cc",
        "support/memory_stats_logger.h",
        "support/profiler_service.cc",
    ],
    copts = ANDROID_COPTS + [
        "-Itools/base/profiler/native",
        "-I$(GENDIR)/tools/base/profiler",
    ],
    linkstatic = 1,
    visibility = ["//visibility:public"],
    deps = [
        "//prebuilts/studio/jdk:jni_headers",
        "//tools/base/profiler/native/utils:profiler_utils",
        "//tools/base/profiler/proto:profiler_cc_proto",
    ],
)

jni_library(
    name = "supportjni",
    visibility = ["//visibility:public"],
    deps = [":libsupportjni.so.stripped"],
)
