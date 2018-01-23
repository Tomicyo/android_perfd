#include "gpu_collector.h"

#include <unistd.h>
#include "utils/trace.h"
#include "utils/clock.h"
#include "utils/stopwatch.h"
#include "utils/thread_name.h"

namespace profiler {

GpuCollector::~GpuCollector() {
  if (is_running_.load()) {
    Stop();
  }
}

void GpuCollector::Start() {
  if (!is_running_.exchange(true)) {
    sampler_thread_ = std::thread(&GpuCollector::Collect, this);
  }
}

void GpuCollector::Stop() {
  if (is_running_.exchange(false)) {
    sampler_thread_.join();
  }
}

void GpuCollector::Collect() {
  SetThreadName("Studio:PollGpu");
  Stopwatch stopwatch;
  while (is_running_.load()) {
    Trace::Begin("GPU:Collect");
    stopwatch.Start();
    usage_sampler_.Sample();
    int64_t elapsed_time_us = Clock::ns_to_us(stopwatch.GetElapsed());
    Trace::End();
    if (sampling_interval_in_us_ > elapsed_time_us) {
      usleep(sampling_interval_in_us_ - elapsed_time_us);
    } else {
      // Do not sleep. Read data for the next round immediately.
    }
  }
}

} // profiler