#include "servicelayerfunctionalities.h"

/*
    Implementing Functionalities for ClientForKeyValueStore
*/
void ClientForKeyValueStore::put(const std::string &key, const std::string &value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);

  chirp::PutReply reply;

  ClientContext context;
  Status status = stub_->put(&context, request, &reply);

  if (status.ok()) {
    std::cout << "status is ok" << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
}
std::string ClientForKeyValueStore::get(const std::string &key) {
  //TODO: Functionality from commandline client that takes a key and call 'get' function from Service Layer
}
void ClientForKeyValueStore::deletekey(const std::string &key) {
  //TODO: Functionality from commandline client that takes a key and 'deletekey' function from service
}
/*
    Implementing Functionalities for Chirp2Impl
*/

Status Chirp2Impl::deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response) {
  //TODO: Takes request from service layer, deletes data from backend storage and then sends a response
  return Status::OK;
}

Status Chirp2Impl::registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  clientKey.put("registeruser", request->username());
  return Status::OK;
}
Status Chirp2Impl::chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) {
  //TODO: Takes a request from service layer, saves the chirp into backend storage and returns a repsonse 
  return Status::OK;
}
Status Chirp2Impl::follow(ServerContext* context, const FollowRequest* request, FollowReply* response) {
  //TODO: Takes a request from service layer, adds a follower to the user in backend storage and returns a repsonse 
  return Status::OK;
}
Status Chirp2Impl::read(ServerContext* context, const ReadRequest* request, ReadReply* response) {
  //TODO: Takes a request from service layer, reads chirps from user in backend storage and returns a repsonse 
  return Status::OK;
}
Status Chirp2Impl::monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter< ::chirp::MonitorReply>* writer) {
  //TODO: Takes a request from service layer, continuiously sends data from backend storage 
  return Status::OK;
}