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
    mess.set_parent_id(request->parent_id());
    mess.SerializeToString(&value);
  }

  clientKey.put(key,value);
  return Status::OK;
}
Status Chirp2Impl::follow(ServerContext* context, const FollowRequest* request, FollowReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  //get information on username from backend
  std::string bytesUserInfo = clientKey.get(request->username());
  //Parse from string
  chirp::User userInfo;
  userInfo.ParseFromString(bytesUserInfo);

  std::string key; 
  {
    chirp::Username user;
    user.set_username(request->username());
    user.SerializeToString(&key);
  }

  std::string value; 
  {
    std::string bytesUserToFollow = clientKey.get(request->to_follow());
    //TODO: check if user to_follow exists

    //create new user to replace the old user info
    chirp::User recreateUser;
    recreateUser.set_username(request->username());

    if(!userInfo.has_followers()) {
      chirp::Followers userFollowers;
      userFollowers.add_followers(request->to_follow());
      recreateUser.set_followers(userFollowers);
      //HOW TO SET MESSAGE TO A MESSAGE?!
    }
    //TODO: Else if it has existing followers
    recreateUser.SerializeToString(&value);
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