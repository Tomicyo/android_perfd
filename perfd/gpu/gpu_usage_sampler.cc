#include "gpu_usage_sampler.h"

#include <unistd.h>
#include <errno.h>
#include <vector>
#include <unordered_map>
#include <functional>

#include "utils/file_reader.h"

using profiler::proto::GpuData;

namespace profiler {
using FQueryGpuUsage = std::function<float(std::string const&)>;
using FVendorGpuUsage = std::unordered_map<std::string, FQueryGpuUsage>;

static FVendorGpuUsage vendor_usage_parse = {
    {"/sys/class/kgsl/kgsl-3d0/gpubusy", [](std::string const& usage_file)->float {
        std::string buffer;
        float usage = 0.0f;
        if (FileReader::Read(usage_file, &buffer)) {
            int used_cycles = 0, total_cycles = 0;
            sscanf(buffer.c_str(), "%d %d\n", &used_cycles, &total_cycles);
            usage = total_cycles > 0 ? (1.0f * used_cycles / total_cycles) : 0;
        }
        return usage;
    }},
    {"/proc/mali/utilization", [](std::string const& usage_file)->float {
        float usage = 0.0f;
        std::string buffer;
        if (FileReader::Read(usage_file, &buffer)) {
            int gpu_usage = 0, _1 =0, _2=0, freq = 0;
            sscanf(buffer.c_str(), "gpu/cljs0/cljs1=%d/%d/%d, frequency=%d(kHz)", &gpu_usage, &_1, &_2, &freq);
            usage = 0.01f * gpu_usage;
        }
        return usage;
    }}
};

GpuUsageSampler::GpuUsageSampler(Daemon::Utilities* utilities, GpuCache* cache)
: clock_(utilities->clock())
, cache_(*cache)
{
    for(auto& pair : vendor_usage_parse)
    {
        std::string& path = pair->first;
        int fd = open(path.c_str(), O_RDONLY);
        if(fd != -1)
        {
            driver_stat_file_ = path;
            close(fd);
            break;
        }
        else
        {
            if(errno == EACCES)
            {
                // permission denied or not exists !
            }
        }
    }
}

GpuUsageSampler::~GpuUsageSampler() {
}

bool GpuUsageSampler::Sample() {
    GpuData data;
    if(!driver_stat_file_.empty()) {
        float usage = vendor_usage_parse[driver_stat_file_](driver_stat_file_);
        data.set_utilization(usage);
    }
    data.mutable_basic_info()->set_process_id(0);
    data.mutable_basic_info()->set_end_timestamp(clock_.GetCurrentTime());
    cache_.Add(data);
    return true;
}

}