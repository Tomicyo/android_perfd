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
        GpuData data;
        data.set_utilization(1.0f);
        *(response->mutable_data()) = data;
        return Status::OK;
    }
}