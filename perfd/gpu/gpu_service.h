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
  GpuServiceImpl(const Clock& clock, GpuCache* gpu_cache, GpuUsageSampler* usage_sampler)
      : clock_(clock)
      , cache_(*gpu_cache)
      , usage_sampler_(*usage_sampler) {}
      
  ~GpuServiceImpl();

  virtual grpc::Status GetData(
    grpc::ServerContext* context, 
    const profiler::proto::GpuDataRequest* request, 
    profiler::proto::GpuDataResponse* response) override;

  private:
    const Clock& clock_;
    // Data cache that will be queried to serve requests.
    GpuCache& cache_;
    GpuUsageSampler& usage_sampler_;
};

}

#endif