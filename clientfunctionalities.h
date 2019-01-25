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
using chirp::Timestamp;
using chirp::Chirp;
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
using chirp::KeyValueStore;
/**
	ClientFunctionalities is a class that the Command Line Client will call when 
	users are registering, sending a chirp, following other chirpers, and etc.
*/
class ClientFunctionalities {
  public:
    ClientFunctionalities(std::shared_ptr<Channel> channel) :stub_(KeyValueStore::NewStub(channel)){}
    void registeruser(std::string username); //Create a new user with username
    void chirp(std::string username, std::string text, std::string parent_id); //add a chirp to this username
    void follow(std::string username, std::string to_follow); //follow a chirper with this username
    void read(std::string chirp_id); //read all the chirps from this user
    void monitor(std::string username); //read all the chirps that their followers chirped

  private:
    std::unique_ptr<KeyValueStore::Stub> stub_; //unsure what this does

};

#endif //CLIENT_FUNCTIONALITIES_H_