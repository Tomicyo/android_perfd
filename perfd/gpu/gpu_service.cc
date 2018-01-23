#include "gpu_service.h"

using profiler::proto::GpuDataRequest;
using profiler::proto::GpuDataResponse;
using profiler::proto::GpuData;

using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

namespace profiler {
    GpuServiceImpl::~GpuServiceImpl() {
    }

    grpc::Status GpuServiceImpl::GetData(ServerContext* context, const GpuDataRequest* request, GpuDataResponse* response) {
        Trace trace("GPU:GetData");
        const vector<GpuData>& data =
            cache_.Retrieve(id, request->start_timestamp(), request->end_timestamp());
        for (const auto& datum : data) {
            *(response->add_data()) = datum;
        }
        return Status::OK;
    }
}