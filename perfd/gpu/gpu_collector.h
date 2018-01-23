#ifndef PERFD_GPU_GPU_COLLECTOR_H_
#define PERFD_GPU_GPU_COLLECTOR_H_

#include <atomic>
#include <cstdint>
#include <thread>

#include "gpu_usage_sampler.h"

namespace profiler {
    class GpuCollector {
    public:
        GpuCollector(int64_t interval_in_us, GpuUsageSampler* usage_sampler)
            : usage_sampler_(*usage_sampler),
              sampling_interval_in_us_(interval_in_us) {}

        ~GpuCollector();

        // Creates a thread that collects and saves data continually.
        // Assumes |Start()| and |Stop()| are called by the same thread.
        void Start();

        // Stops collecting data and wait for thread exit.
        // Assumes |Start()| and |Stop()| are called by the same thread.
        void Stop();

    private:
        void Collect();

        // Thread that sampling operations run on.
        std::thread sampler_thread_;
        // True if sampling operations is running.
        std::atomic_bool is_running_{false};
        // Holder of sampler operations.
        GpuUsageSampler& usage_sampler_;
        // Sampling window size in microseconds.
        int64_t sampling_interval_in_us_;
    };
}

#endif