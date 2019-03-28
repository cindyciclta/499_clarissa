#include "keyvaluestoreserver.h"

Status KeyValueStoreServer::put(ServerContext *context,
                                const PutRequest *request, PutReply *response) {
  kvstore_.Put(request->key(), request->value());
  std::cout << "Sucessful: put()" << std::endl;
  return Status::OK;
}

Status KeyValueStoreServer::get(
    ServerContext *context,
    grpc::ServerReaderWriter<GetReply, GetRequest> *stream) {
  GetRequest request;
  bool found = false;

  while (stream->Read(&request)) {
    auto vector_from_get = kvstore_.Get(request.key());
    if (vector_from_get.size() > 0) {
      GetReply reply;
      reply.set_value(vector_from_get[0]);
      found = true;
      stream->Write(reply);
    }
  }
  if (!found) {
    return Status::CANCELLED;
  }
  std::cout << "Sucessful: get()" << std::endl;
  return Status::OK;
}

Status KeyValueStoreServer::deletekey(ServerContext *context,
                                      const DeleteRequest *request,
                                      DeleteReply *response) {
  if (kvstore_.DeleteKey(request->key())) {
    return Status::OK;
  }
  std::cout << "Sucessful: deletekey()" << std::endl;
  return Status::CANCELLED;
}
