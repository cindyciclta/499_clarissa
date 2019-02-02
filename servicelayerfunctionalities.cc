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
  chirp::GetRequest request;
  request.set_key(key);

  ClientContext context;
  std::shared_ptr<grpc::ClientReaderWriter <GetRequest, GetReply> > stream(stub_->get(&context));

  stream->Write(request);
  stream->WritesDone();

  chirp::GetReply reply;
  while (stream->Read(&reply)) {
    std::cout << "GOT SOMETHING "<< std::endl;
  }

  Status status = stream->Finish();
  if (!status.ok()) {
    std::cout << "status is ok from get" << std::endl;
  }
  else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
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
    chirp::User user;
    user.set_username(request->username());
    user.SerializeToString(&value);
  }
  clientKey.put(key, value);
  return Status::OK;
}
Status Chirp2Impl::chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string key; 
  {
    chirp::ID id;
    //TODO: I believe that I need to figure out if this Chirp is a new Chirp or a reply Chirp
    id.set_id(request->parent_id());
    id.SerializeToString(&key);
  }

  std::string value; 
  {
    chirp::Chirp mess;
    mess.set_username(request->username());
    mess.set_text(request->text());
    mess.set_id(std::to_string(chirps_ ) );
    chirps_++;
    mess.set_parent_id(request->parent_id());
    mess.SerializeToString(&value);
  }
  //Serializing: tested in this location
  std::string test;
  {
    chirp::Chirp c;
    c.ParseFromString(value);
    std::cout << c.text() <<std::endl;
  }

  clientKey.put(key,value);
  return Status::OK;
}
Status Chirp2Impl::follow(ServerContext* context, const FollowRequest* request, FollowReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  std::string key; 
  {
    chirp::Username user;
    user.set_username(request->username());
    user.SerializeToString(&key);
  }
  std::string test;
  {
    std::string getKey = clientKey.get(request->username());

    chirp::Username user;
    user.ParseFromString(key);
    std::cout << user.username() <<std::endl;
  }
  std::string value; 
  {
    //TODO: Send User message back with added follower

    chirp::User user;
    user.SerializeToString(&value);
  }

  clientKey.put(key,value);
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