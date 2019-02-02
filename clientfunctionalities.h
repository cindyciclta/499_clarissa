#ifndef CLIENT_FUNCTIONALITIES_H_
#define CLIENT_FUNCTIONALITIES_H_
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using chirp::RegisterRequest;
using chirp::RegisterReply;
using chirp::ChirpRequest;
using chirp::ChirpReply;
using chirp::FollowRequest;
using chirp::FollowReply;
using chirp::ReadRequest;
using chirp::ReadReply;
using chirp::MonitorRequest;
using chirp::MonitorReply;
using chirp::ServiceLayer;
using chirp::KeyValueStore;
/**
	ClientFunctionalities is a class that the Command Line Client will call when 
	users are registering, sending a chirp, following other chirpers, and etc.
*/
class ClientFunctionalities {
 public:
  //Constructor take takes in a shared pointer of Channel
  explicit ClientFunctionalities(const std::shared_ptr<Channel> &channel) :stub_(chirp::ServiceLayer::NewStub(channel)) {}
  //Create a new user with username
  void registeruser(const std::string &username); 
  //add a chirp to this username
  void chirp(const std::string &username, const std::string &text, const std::string &parent_id); 
  //follow a chirper with this username
  void follow(const std::string &username, const std::string &to_follow); 
  //read all the chirps from this user
  void read(const std::string &chirp_id); 
  //read all the chirps that their followers chirped
  void monitor(const std::string &username); 
 private:
  //unsure what this does
  std::unique_ptr<chirp::ServiceLayer::Stub> stub_; 
};

#endif //CLIENT_FUNCTIONALITIES_H_