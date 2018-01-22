#ifndef PERFD_GPU_GPU_COLLECTOR_H_
#define PERFD_GPU_GPU_COLLECTOR_H_

#include <atomic>
#include <cstdint>
#include <thread>

namespace profiler {
    class GpuCollector {
    public:
        GpuCollector(int64_t interval_in_us, CpuUsageSampler* usage_sampler,
                    ThreadMonitor* thread_monitor)
            : usage_sampler_(*usage_sampler),
              thread_monitor_(*thread_monitor),
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
        //CpuUsageSampler& usage_sampler_;
        // Monitor of thread activities.
        //ThreadMonitor& thread_monitor_;
        // Sampling window size in microseconds.
        int64_t sampling_interval_in_us_;
    };
}

#endif