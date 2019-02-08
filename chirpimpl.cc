#include "chirpimpl.h"

Status ChirpImpl::put(ServerContext* context, const PutRequest* request, PutReply* response) {
  //TODO: Recieving request from service layer and saving object into backend and returning a response
  std::lock_guard<std::mutex> lock(mymutex_);
  addkey(request->key(), request->value());
  return Status::OK;
}

Status ChirpImpl::get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
  //TODO: Streaming chirps to user
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

void ChirpImpl::addkey(const std::string &key, const std::string &value) {
  auto it = data_.find(key);
  if(it != data_.end()) {
    it->second = value;
  } else {
    data_.emplace(key, value);
  }
   
}

Status ChirpImpl::deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response) {
  //TODO: Deletes info from backend storage

  return Status::OK;
}
