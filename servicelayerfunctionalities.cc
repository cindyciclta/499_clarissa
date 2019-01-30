#include "servicelayerfunctionalities.h"

/*
    Implementing Functionalities for ClientForKeyValueStore
*/
void ClientForKeyValueStore::put(const std::string &key, const std::string &value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  std::cout << "TEST1"<<std::endl;
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

Status Chirp2Impl::registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string key; 
  {
    chirp::Username user;
    user.set_username(request->username());
    user.SerializeToString(&key);
  }
  std::string value; 
  {
    request->SerializeToString(&value);
  }
 
  clientKey.put(key, value);
  return Status::OK;
}
Status Chirp2Impl::chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string key; 
  {
    chirp::ID id;
    id.set_id(request->parent_id());
    id.SerializeToString(&key);
  }
  std::string value; 
  {
    chirp::Chirp mess;
    mess.set_username(request->username());
    mess.set_text(request->text());
    mess.set_id(std::to_string(chirps_ ) );
    mess.set_parent_id(request->parent_id());
    request->SerializeToString(&value);
  }
  
  clientKey.put(key,value);
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