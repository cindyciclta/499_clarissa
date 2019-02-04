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
  /*
    Tested: Able to add chirps! "chip<id>" is key and message Proto Chirp is value 
  */

  chirp::User user;
  std::string getValue = clientKey.get(request->username());
  user.ParseFromString(getValue);
  std::string userKey;

  std::string value; 
  {
    chirp::Chirp* message = user.add_chirps();
    message->set_username(request->username());
    message->set_text(request->text());
    message->set_id(std::to_string(chirps_ ) );
    chirps_++;
    //TODO: Timestamp
    message->set_parent_id(request->parent_id());
    message->SerializeToString(&value);
    user.SerializeToString(&userKey);
  }

  if (request->parent_id() == "none") {
    clientKey.put("chirp"+std::to_string(chirps_),value);
  } else {
    clientKey.put("reply"+request->parent_id(),value);
  }
  clientKey.put(request->username(),userKey);
  
  std::string test;
  {
    chirp::User user;
    std::string getValue = clientKey.get(request->username());
    user.ParseFromString(getValue);

    std::cout << "ULTIMATE TEST: "<< user.chirps(0).text() <<std::endl;
  }

  return Status::OK;
}

Status Chirp2Impl::read(ServerContext* context, const ReadRequest* request, ReadReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string chirpmsg = clientKey.get("chirp"+request->chirp_id());//assume chip_id is an a number
  std::string test;
  {
    chirp::Chirp c;
    c.ParseFromString(chirpmsg);
    std::cout << c.text() <<std::endl;
  }

  return Status::OK;
}
Status Chirp2Impl::monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter< ::chirp::MonitorReply>* writer) {
  //TODO: Takes a request from service layer, continuiously sends data from backend storage 
  return Status::OK;
}