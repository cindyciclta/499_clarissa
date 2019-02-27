#ifndef CLIENT_FUNCTIONALITIES_H_
#define CLIENT_FUNCTIONALITIES_H_
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <map>
#include <ctime>
#include <ratio>
#include <chrono>

#include <grpcpp/grpcpp.h>
#include <grpcpp/client_context.h>
#include "chirp.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ServerBuilder;
using grpc::ServerContext;

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
  /*
    Constructor take takes in a shared pointer of Channel
  */
  explicit ClientFunctionalities(const std::shared_ptr<Channel> &channel) 
                                  : stub_(chirp::ServiceLayer::NewStub(channel)) {}
  /*
    Create a new user with username
  */
  void RegisterUser(const std::string &username); 
  /*
    Add a chirp to this username
  */
  void Chirp(const std::string &username, const std::string &text, const std::string &parent_id); 
  /*
    Follow a chirper with this username
  */
  void Follow(const std::string &username, const std::string &to_follow); 
  /*
    Read all the chirps from this user
  */
  void Read(const std::string &chirp_id); 
  /*
    Read all the chirps that their followers chirps in real-time
  */
  void Monitor(const std::string &username); 
 private:
  /*
    Unique ptr for all the stub
  */
  std::unique_ptr<chirp::ServiceLayer::Stub> stub_; 
};

#endif //CLIENT_FUNCTIONALITIES_H_