#include "keyvaluestoreserver.h"

Status KeyValueStoreServer::put(ServerContext* context, const PutRequest* request, PutReply* response) {
  std::lock_guard<std::mutex> lock(mymutex_);
  addkey(request->key(), request->value());
  return Status::OK;
}

Status KeyValueStoreServer::get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
  std::lock_guard<std::mutex> lock(mymutex_);
  GetRequest request;
  bool found = false;

  while (stream->Read(&request)) {
    auto it = data_.find(request.key());
    for(const auto &i :data_) {
      std::cout << "key: "<< i.first <<std::endl;
    }
    if (it != data_.end()) {
      GetReply reply;
      reply.set_value(it->second);
      found = true;
      stream->Write(reply);
    } else {
      std::cout << "Not Found Key"<<std::endl;
    }
  }
  if (!found) {
    return Status::CANCELLED;
  }
  std:: cout << std::endl;
  return Status::OK;
}

void KeyValueStoreServer::addkey(const std::string &key, const std::string &value) {
  auto it = data_.find(key);
  if(it != data_.end()) {
    it->second = value;
  } else {
    data_.emplace(key, value);
  }
   
}

Status KeyValueStoreServer::deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response) {
  //TODO: Deletes info from backend storage

  return Status::OK;
}
