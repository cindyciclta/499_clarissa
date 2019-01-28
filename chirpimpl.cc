#include "chirpimpl.h"

Status ChirpImpl::put(ServerContext* context, const PutRequest* request, PutReply* response) {
  //TODO: Recieving request from service layer and saving object into backend and returning a response
  std::lock_guard<std::mutex> lock(mymutex_);
  //Question: how do you determine if a request is 'registeruser' or something else?
  if(request->key() == "registeruser") {
    User newUser(request->value(), data_.size()+1); 
    data_.emplace(request->value(),newUser);
  }
  return Status::OK;
}
Status ChirpImpl::get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
  //TODO: Streaming chirps to user
  std::vector<GetRequest> received_notes;
  GetRequest request;
  while (stream->Read(&request)) {
    for (const GetRequest& r : received_notes) {
      //TODO: Continuously sends data through stream
    }
    received_notes.push_back(request);
  }
  return Status::OK;
}


Status ChirpImpl::Delete(ServerContext* context, const DeleteRequest* request, DeleteReply* response) {
  //TODO: Deletes info from backend storage
  return Status::OK;
}