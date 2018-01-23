#include "gpu_cache.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <mutex>
#include <vector>

#include "proto/profiler.pb.h"

using profiler::proto::GpuData;
using std::vector;

namespace profiler {

bool GpuCache::AllocateAppCache(int32_t app_id) {
  if (FindAppCache(app_id) != nullptr) return true;
  app_caches_.emplace_back(new AppGpuCache(app_id, capacity_));
  return true;
}

bool GpuCache::DeallocateAppCache(int32_t app_id) {
  for (auto it = app_caches_.begin(); it != app_caches_.end(); it++) {
    if (app_id == (*it)->app_id) {
      app_caches_.erase(it);
      return true;
    }
  }
  return false;
}

bool GpuCache::Add(const GpuData& datum) {
  int32_t app_id = datum.basic_info().process_id();
  auto* found = FindAppCache(app_id);
  if (found == nullptr) return false;
  found->usage_cache.Add(datum, datum.basic_info().end_timestamp());
  return true;
}

vector<GpuData> GpuCache::Retrieve(int64_t from, int64_t to) {
  auto* found = FindAppCache(app_id);
  if (found == nullptr) {
    vector<GpuData> empty;
    return empty;
  }
  return found->usage_cache.GetValues(from, to);
}

GpuCache::AppGpuCache* CpuCache::FindAppCache(int32_t app_id) {
  for (auto& cache : app_caches_) {
    if (app_id == cache->app_id) {
      return cache.get();
    }
  }
  return nullptr;
}

}