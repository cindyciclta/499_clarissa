#ifndef SERVICE_LAYER_FUNCTIONALITIES_H_
#define SERVICE_LAYER_FUNCTIONALITIES_H_
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/client_context.h>
#include "chirp.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using chirp::Chirp;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::PutReply;
using chirp::PutRequest;
using chirp::GetRequest;
using chirp::DeleteRequest;
using chirp::DeleteReply;
using chirp::KeyValueStore;

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

/*
    ClientForKeyValueStore class is for the functionalities that the service layer will call when 
    it wants to add or get or delete things from the backend
*/
class ClientForKeyValueStore {
 public:
  explicit ClientForKeyValueStore(std::shared_ptr<Channel> channel) :stub_(KeyValueStore::NewStub(channel)) {}
  //put data into backend
  void put(const std::string &key, const std::string &value); 
  //get information from backend
  std::string get(const std::string &key);
  //delete infromation in backend 
  void deletekey(const std::string &key); 
  
 private:
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

/*
    Chirp2Impl class is for the functionalties that would be called from the ClientCommandLine client. 
    It will handle requests from the clientcommandline client and sends a response
*/
class Chirp2Impl final : public ServiceLayer ::Service {
 public:
  //Deletes users in backend
  Status registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response);
  //add chirps into backend storage
  Status chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response); 
  //add a follower to a specific user to backend storage
  Status follow(ServerContext* context, const FollowRequest* request, FollowReply* response); 
  //reads chirps from a user stored in backend
  Status read(ServerContext* context, const ReadRequest* request, ReadReply* response); 
  //continuously streaming chirps from all followers all saved in the backend
  Status monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter< ::chirp::MonitorReply>* writer); 
 private:
  int chirps_ = 0; //Total umber of chirps 
};

#endif // SERVICE_LAYER_FUNCTIONALITIES_H_