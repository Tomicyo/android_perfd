#ifndef PERFD_GPU_GPU_USAGE_SAMPLER_H_
#define PERFD_GPU_GPU_USAGE_SAMPLER_H_

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_set>

namespace profiler {
    class GpuUsageSampler {
    public:
        GpuUsageSampler();
        ~GpuUsageSampler();
    };
}

#endif