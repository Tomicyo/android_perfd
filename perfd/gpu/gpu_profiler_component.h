
#ifndef PERFD_GPU_GPU_PROFILER_COMPONENT_H_
#define PERFD_GPU_GPU_PROFILER_COMPONENT_H_

#include "perfd/gpu/gpu_collector.h"
#include "perfd/gpu/gpu_service.h"
#include "perfd/daemon.h"
#include "perfd/profiler_component.h"

namespace profiler {

class GpuProfilerComponent final : public ProfilerComponent {
 private:
  // Default collection interval is 200 milliseconds, i.e., 0.2 second.
  static const int64_t kDefaultCollectionIntervalUs = Clock::ms_to_us(200);
  // The length of data kept by the CPU component in the daemon.
  static const int64_t kSecondsToBuffer = 5;
  // In CPU cache, a datum is added at each collection event which happens
  // in every collection interval. Dividing the length of history we want
  // to keep by the interval leads to the capacity. We add the capacity
  // by 1 to round up the division.
  static const int64_t kBufferCapacity =
      Clock::s_to_us(kSecondsToBuffer) / kDefaultCollectionIntervalUs + 1;

 public:
  // Creates a CPU perfd component and starts sampling right away.
  explicit GpuProfilerComponent(Daemon::Utilities* utilities)
      : clock_(utilities->clock()),
        usage_sampler_(utilities, &cache_) {
    collector_.Start();
  }

  // Returns the service that talks to desktop clients (e.g., Studio).
  grpc::Service* GetPublicService() override { return &public_service_; }

  // Returns the service that talks to device clients (e.g., the agent).
  grpc::Service* GetInternalService() override { return nullptr; }

 private:
  const Clock& clock_;
  GpuCache cache_{kBufferCapacity};
  GpuUsageSampler usage_sampler_;
  // gpu collector
  GpuCollector collector_{kDefaultCollectionIntervalUs, &usage_sampler_};
  GpuServiceImpl public_service_{clock_, &cache_, &usage_sampler_};
};

}

#endif
