#include "servicelayerfunctionalities.h"

/*
    Implementing Functionalities for ClientForKeyValueStore
*/
void ClientForKeyValueStore::put(const std::string &key, const std::string &value) {
  std::cout << std::endl;
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  ClientContext context;
  Status status = stub_->put(&context, request, &reply);

  if (status.ok()) {
    std::cout << "status is ok" << std::endl;
  } else {
    std::cout << "Problem with put function of service layer"<<std::endl;
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
  stream->Read(&reply);

  Status status = stream->Finish();

  if (status.ok()) {
    std::cout << "status is ok from get" << std::endl;
  }
  else {
   std::cout << "Problem in get function of service layer "<< std::endl;
  }
  return reply.value();
}
void ClientForKeyValueStore::deletekey(const std::string &key) {
  //TODO: Functionality from commandline client that takes a key and 'deletekey' function from service
}
/*
    Implementing Functionalities for Chirp2Impl
*/

Status Chirp2Impl::registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  /* 
    Tested: Able to add new users
  */
  std::string value; 
  {
    chirp::User userValue;
    userValue.set_username(request->username()); 
    userValue.SerializeToString(&value);
  }
  clientKey.put(request->username(), value);
  return Status::OK;
}

Status Chirp2Impl::follow(ServerContext* context, const FollowRequest* request, FollowReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  /* 
    Tested: Able to add followers to user with an empty followers list and if there exist a followers list
  */
        
  std::string value;
  {
    chirp::User user;
    std::string getKey = clientKey.get(request->username());
    user.ParseFromString(getKey);
    chirp::Followers* f = user.mutable_followers();
    f->add_username(request->to_follow());
    user.SerializeToString(&value);
  }

  clientKey.put(request->username(),value);
  return Status::OK;
}

Status Chirp2Impl::chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  std::string value; 
  {
    chirp::Chirp message;
    message.set_username(request->username());
    message.set_text(request->text());
    message.set_id(std::to_string(chirps_ ) );
    chirps_++;
    message.set_parent_id(request->parent_id());
    message.SerializeToString(&value);
  }
  //Serializing: tested in this location
  std::string test;
  {
    chirp::Chirp c;
    c.ParseFromString(value);
    std::cout << c.text() <<std::endl;
  }

  clientKey.put("chirp"+std::to_string(chirps_-1),value);
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