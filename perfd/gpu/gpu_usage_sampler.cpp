#include "gpu_usage_sampler.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "utils/file_reader.h"

using FQueryGpuUsage = std::functional<float(std::string const&)>;
using FVendorGpuUsage = std::unordered_map<std::string, FQueryGpuUsage>;

static FVendorGpuUsage vendor_usage_parse = {
    {"/sys/class/kgsl/kgsl-3d0/gpubusy", [](std::string const& usage_file)->float {
        string buffer;
        if (FileReader::Read(usage_file, &buffer)) {

        }
    }},
    {"/proc/mali/utilization", [](std::string const& usage_file)->float {
        string buffer;
        if (FileReader::Read(usage_file, &buffer)) {

        }
    }}
};