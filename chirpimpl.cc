#include "chirpimpl.h"

Status ChirpImpl::put(ServerContext* context, const PutRequest* request, PutReply* response) {
  //TODO: Recieving request from service layer and saving object into backend and returning a response
  std::lock_guard<std::mutex> lock(mymutex_);
  data_.emplace(request->key(), request->value());
  return Status::OK;
}
Status ChirpImpl::get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
  //TODO: Streaming chirps to user

  GetRequest request;
  while (stream->Read(&request)) {

      auto it = data_.find( request.key());
      if(it != data_.end()) {
        std::cout << "Found Key"<<std::endl;
        GetReply reply;
        std::string temp = it->second;
        reply.set_value(temp);
        stream->Write(reply);
      }else {
        std::cout << "Not Found Key"<<std::endl;
      }
  }
  return Status::OK;
}

Status ChirpImpl::deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response) {
  //TODO: Deletes info from backend storage
  return Status::OK;
}