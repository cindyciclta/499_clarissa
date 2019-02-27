#include "keyvaluestoreserver.h"

Status KeyValueStoreServer::Put(ServerContext* context, const PutRequest* request, PutReply* response) {
  std::lock_guard<std::mutex> lock(mymutex_);
  AddKey(request->key(), request->value());
  std::cout << "Sucessful: put()" <<std::endl;
  return Status::OK;
}

Status KeyValueStoreServer::Get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
  std::lock_guard<std::mutex> lock(mymutex_);
  GetRequest request;
  bool found = false;

  while (stream->Read(&request)) {
    auto it = data_.find(request.key());
    if (it != data_.end()) {
      GetReply reply;
      reply.set_value(it->second);
      found = true;
      stream->Write(reply);
    }
  }
  if (!found) {
    return Status::CANCELLED;
  }
  std::cout << "Sucessful: get()" <<std::endl;
  return Status::OK;
}

Status KeyValueStoreServer::DeleteKey(ServerContext* context, const DeleteRequest* request, DeleteReply* response) {
  std::lock_guard<std::mutex> lock(mymutex_);
  if (DeleteKeyHelper(request->key())) {
    std::cout << "Sucessful: deletekey()" <<std::endl;
    return Status::OK;
  }
  return Status::CANCELLED;
}

bool KeyValueStoreServer::DeleteKeyHelper(const std::string &key) {
  auto it = data_.find(key);
  if(it != data_.end()) {
    data_.erase(it);
    return true;
  }
  return false;
}

void KeyValueStoreServer::AddKey(const std::string &key, const std::string &value) {
  auto it = data_.find(key);
  if(it != data_.end()) {
    it->second = value;
  } else {
    data_.emplace(key, value);
  }
}
