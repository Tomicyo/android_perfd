#ifndef PERFD_GPU_GPU_USAGE_SAMPLER_H_
#define PERFD_GPU_GPU_USAGE_SAMPLER_H_

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>

#include "perfd/daemon.h"
#include "gpu_cache.h"
#include "utils/clock.h"

namespace profiler {
    class GpuUsageSampler {
    public:
        GpuUsageSampler(Daemon::Utilities* utilities, GpuCache *cache);
        ~GpuUsageSampler();

        bool Sample();
    private:
        const Clock& clock_;
        // Cache where collected data will be saved.
        GpuCache& cache_;
        std::string driver_stat_file_;
    };
}

#endif