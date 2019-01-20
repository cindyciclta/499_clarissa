// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: chirp.proto

#include "chirp.pb.h"
#include "chirp.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace chirp {

static const char* KeyValueStore_method_names[] = {
  "/chirp.KeyValueStore/put",
  "/chirp.KeyValueStore/get",
  "/chirp.KeyValueStore/Delete",
};

std::unique_ptr< KeyValueStore::Stub> KeyValueStore::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< KeyValueStore::Stub> stub(new KeyValueStore::Stub(channel));
  return stub;
}

KeyValueStore::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_put_(KeyValueStore_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_get_(KeyValueStore_method_names[1], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_Delete_(KeyValueStore_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status KeyValueStore::Stub::put(::grpc::ClientContext* context, const ::chirp::PutRequest& request, ::chirp::PutReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_put_, context, request, response);
}

void KeyValueStore::Stub::experimental_async::put(::grpc::ClientContext* context, const ::chirp::PutRequest* request, ::chirp::PutReply* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_put_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::chirp::PutReply>* KeyValueStore::Stub::AsyncputRaw(::grpc::ClientContext* context, const ::chirp::PutRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::chirp::PutReply>::Create(channel_.get(), cq, rpcmethod_put_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::chirp::PutReply>* KeyValueStore::Stub::PrepareAsyncputRaw(::grpc::ClientContext* context, const ::chirp::PutRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::chirp::PutReply>::Create(channel_.get(), cq, rpcmethod_put_, context, request, false);
}

::grpc::ClientReaderWriter< ::chirp::GetRequest, ::chirp::GetReply>* KeyValueStore::Stub::getRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::chirp::GetRequest, ::chirp::GetReply>::Create(channel_.get(), rpcmethod_get_, context);
}

::grpc::ClientAsyncReaderWriter< ::chirp::GetRequest, ::chirp::GetReply>* KeyValueStore::Stub::AsyncgetRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::chirp::GetRequest, ::chirp::GetReply>::Create(channel_.get(), cq, rpcmethod_get_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::chirp::GetRequest, ::chirp::GetReply>* KeyValueStore::Stub::PrepareAsyncgetRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::chirp::GetRequest, ::chirp::GetReply>::Create(channel_.get(), cq, rpcmethod_get_, context, false, nullptr);
}

::grpc::Status KeyValueStore::Stub::Delete(::grpc::ClientContext* context, const ::chirp::DeleteRequest& request, ::chirp::DeleteReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Delete_, context, request, response);
}

void KeyValueStore::Stub::experimental_async::Delete(::grpc::ClientContext* context, const ::chirp::DeleteRequest* request, ::chirp::DeleteReply* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Delete_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::chirp::DeleteReply>* KeyValueStore::Stub::AsyncDeleteRaw(::grpc::ClientContext* context, const ::chirp::DeleteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::chirp::DeleteReply>::Create(channel_.get(), cq, rpcmethod_Delete_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::chirp::DeleteReply>* KeyValueStore::Stub::PrepareAsyncDeleteRaw(::grpc::ClientContext* context, const ::chirp::DeleteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::chirp::DeleteReply>::Create(channel_.get(), cq, rpcmethod_Delete_, context, request, false);
}

KeyValueStore::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KeyValueStore_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KeyValueStore::Service, ::chirp::PutRequest, ::chirp::PutReply>(
          std::mem_fn(&KeyValueStore::Service::put), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KeyValueStore_method_names[1],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< KeyValueStore::Service, ::chirp::GetRequest, ::chirp::GetReply>(
          std::mem_fn(&KeyValueStore::Service::get), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KeyValueStore_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KeyValueStore::Service, ::chirp::DeleteRequest, ::chirp::DeleteReply>(
          std::mem_fn(&KeyValueStore::Service::Delete), this)));
}

KeyValueStore::Service::~Service() {
}

::grpc::Status KeyValueStore::Service::put(::grpc::ServerContext* context, const ::chirp::PutRequest* request, ::chirp::PutReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status KeyValueStore::Service::get(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::chirp::GetReply, ::chirp::GetRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status KeyValueStore::Service::Delete(::grpc::ServerContext* context, const ::chirp::DeleteRequest* request, ::chirp::DeleteReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace chirp
