#ifndef PERFD_GPU_GPU_PROFILER_SERVICE_H_
#define PERFD_GPU_GPU_PROFILER_SERVICE_H_


#include <grpc++/grpc++.h>
#include <map>
#include <string>

#include "perfd/gpu/gpu_usage_sampler.h"
#include "proto/gpu.grpc.pb.h"

namespace profiler {
class GpuServiceImpl final : public profiler::proto::GpuService::Service {
 public:
  GpuServiceImpl(const Clock& clock, 
                 GpuUsageSampler* usage_sampler, ThreadMonitor* thread_monitor)
      : 
        clock_(clock),
        usage_sampler_(*usage_sampler),
        thread_monitor_(*thread_monitor),
        simplerperf_manager_(clock) {}
  ~GpuServiceImpl();

  virtual grpc::Status GetData(grpc::ServerContext* context, const profiler::proto::GpuDataRequest* request, profiler::proto::GpuDataResponse* response) override;
};

}

#endif