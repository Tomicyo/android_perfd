#ifndef PERFD_GPU_GPU_CACHE_H_
#define PERFD_GPU_GPU_CACHE_H_

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

#include "proto/gpu.grpc.pb.h"
#include "proto/gpu.pb.h"
#include "utils/time_value_buffer.h"

namespace profiler {

class GpuCache {
 public:
  // Construct the main CPU cache holder. |capacity| is of every app's every
  // kind of cache (same size for all).
  explicit GpuCache(int32_t capacity) : capacity_(capacity) {}

  // Returns true if successfully allocating a cache for a given app, or if
  // the cache is already allocated.
  bool AllocateAppCache(int32_t app_id);
  // Returns true if successfully deallocating the cache for a given app.
  bool DeallocateAppCache(int32_t app_id);

  // Returns true if successfully adding |datum| to the cache.
  bool Add(const profiler::proto::GpuData& datum);
  // Retrieves gpu data of with timestamps in interval (|from|, |to|].
  std::vector<profiler::proto::GpuData> Retrieve(int64_t from, int64_t to);

 private:
  // Each app's cache held by CPU component in the on-device daemon.
  struct AppGpuCache {
    int32_t app_id;
    TimeValueBuffer<profiler::proto::GpuData> usage_cache;
    AppGpuCache(int32_t app_id, int32_t capacity)
        : app_id(app_id)
        , usage_cache(capacity, app_id) {}
  };

  // Returns the raw pointer to the cache for a given app. Returns null if
  // it doesn't exist. No ownership transfer.
  AppGpuCache* FindAppCache(int32_t app_id);

  // Each app has a set of dedicated caches.
  std::vector<std::unique_ptr<AppGpuCache>> app_caches_;
  // The capacity of every kind of cache.
  int32_t capacity_;
};

}  // namespace profiler


#endif