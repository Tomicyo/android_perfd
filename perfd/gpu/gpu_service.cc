#include "gpu_service.h"
#include "utils/trace.h"

using profiler::proto::GpuDataRequest;
using profiler::proto::GpuDataResponse;
using profiler::proto::GpuData;

using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

using std::vector;

namespace profiler {
    GpuServiceImpl::~GpuServiceImpl() {
    }

    grpc::Status GpuServiceImpl::GetData(ServerContext* context, const GpuDataRequest* request, GpuDataResponse* response) {
        Trace trace("GPU:GetData");
        //request->app_package_name() ; to pid
        const vector<GpuData>& data =
            cache_.Retrieve(request->start_timestamp(), request->end_timestamp());
        for (const auto& datum : data) {
            *(response->add_data()) = datum;
        }
        return Status::OK;
    }
}