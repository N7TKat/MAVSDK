// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: component_metadata_server/component_metadata_server.proto

#include "component_metadata_server/component_metadata_server.pb.h"
#include "component_metadata_server/component_metadata_server.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace mavsdk {
namespace rpc {
namespace component_metadata_server {

static const char* ComponentMetadataServerService_method_names[] = {
  "/mavsdk.rpc.component_metadata_server.ComponentMetadataServerService/SetMetadata",
};

std::unique_ptr< ComponentMetadataServerService::Stub> ComponentMetadataServerService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ComponentMetadataServerService::Stub> stub(new ComponentMetadataServerService::Stub(channel, options));
  return stub;
}

ComponentMetadataServerService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_SetMetadata_(ComponentMetadataServerService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status ComponentMetadataServerService::Stub::SetMetadata(::grpc::ClientContext* context, const ::mavsdk::rpc::component_metadata_server::SetMetadataRequest& request, ::mavsdk::rpc::component_metadata_server::SetMetadataResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::mavsdk::rpc::component_metadata_server::SetMetadataRequest, ::mavsdk::rpc::component_metadata_server::SetMetadataResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SetMetadata_, context, request, response);
}

void ComponentMetadataServerService::Stub::async::SetMetadata(::grpc::ClientContext* context, const ::mavsdk::rpc::component_metadata_server::SetMetadataRequest* request, ::mavsdk::rpc::component_metadata_server::SetMetadataResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::mavsdk::rpc::component_metadata_server::SetMetadataRequest, ::mavsdk::rpc::component_metadata_server::SetMetadataResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SetMetadata_, context, request, response, std::move(f));
}

void ComponentMetadataServerService::Stub::async::SetMetadata(::grpc::ClientContext* context, const ::mavsdk::rpc::component_metadata_server::SetMetadataRequest* request, ::mavsdk::rpc::component_metadata_server::SetMetadataResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SetMetadata_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::mavsdk::rpc::component_metadata_server::SetMetadataResponse>* ComponentMetadataServerService::Stub::PrepareAsyncSetMetadataRaw(::grpc::ClientContext* context, const ::mavsdk::rpc::component_metadata_server::SetMetadataRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::mavsdk::rpc::component_metadata_server::SetMetadataResponse, ::mavsdk::rpc::component_metadata_server::SetMetadataRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SetMetadata_, context, request);
}

::grpc::ClientAsyncResponseReader< ::mavsdk::rpc::component_metadata_server::SetMetadataResponse>* ComponentMetadataServerService::Stub::AsyncSetMetadataRaw(::grpc::ClientContext* context, const ::mavsdk::rpc::component_metadata_server::SetMetadataRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSetMetadataRaw(context, request, cq);
  result->StartCall();
  return result;
}

ComponentMetadataServerService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ComponentMetadataServerService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ComponentMetadataServerService::Service, ::mavsdk::rpc::component_metadata_server::SetMetadataRequest, ::mavsdk::rpc::component_metadata_server::SetMetadataResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](ComponentMetadataServerService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::mavsdk::rpc::component_metadata_server::SetMetadataRequest* req,
             ::mavsdk::rpc::component_metadata_server::SetMetadataResponse* resp) {
               return service->SetMetadata(ctx, req, resp);
             }, this)));
}

ComponentMetadataServerService::Service::~Service() {
}

::grpc::Status ComponentMetadataServerService::Service::SetMetadata(::grpc::ServerContext* context, const ::mavsdk::rpc::component_metadata_server::SetMetadataRequest* request, ::mavsdk::rpc::component_metadata_server::SetMetadataResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace mavsdk
}  // namespace rpc
}  // namespace component_metadata_server

