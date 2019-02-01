#include "chirpimpl.h"

Status ChirpImpl::put(ServerContext* context, const PutRequest* request, PutReply* response) {
  //TODO: Recieving request from service layer and saving object into backend and returning a response
  std::lock_guard<std::mutex> lock(mymutex_);
  data_.emplace(request->key(), request->value());
  std::cout << "Size data_ "<< data_.size() <<std::endl;
  {
    std::cout << "TESTING " << request->value() << std::endl;
    std::cout << std::endl;
    // std::string temp = request->value();
    // chirp::User user;
    // user.ParseFromString(temp);
    // std::cout << user.username() <<std::endl;
  }
  return Status::OK;
}
Status ChirpImpl::get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
  //TODO: Streaming chirps to user
  std::vector<GetRequest> received_requests;
  GetRequest request;
  while (stream->Read(&request)) {
    for (const GetRequest& r : received_requests) {
      //TODO: Continuously sends data through stream
      for(const auto &i: data_) {
        //if the key in the data_ is the key wanted, return it to the client (service layer)
        if(i.first == r.key()) {
          GetReply reply;
          reply.set_value(i.second);
          stream->Write(reply);
        }
      }
    }
    received_requests.push_back(request);
  }
  return Status::OK;
}

Status ChirpImpl::deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response) {
  //TODO: Deletes info from backend storage
  return Status::OK;
}