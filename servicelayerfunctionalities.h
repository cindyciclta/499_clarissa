#ifndef SERVICE_LAYER_FUNCTIONALITIES_H_
#define SERVICE_LAYER_FUNCTIONALITIES_H_
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"
#include <gtest/gtest.h>

#include "User.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chirp::Chirp;
using chirp::KeyValueStore;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::PutReply;
using chirp::PutRequest;
using chirp::GetRequest;
using chirp::DeleteRequest;
using chirp::DeleteReply;
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

/*
    ClientForKeyValueStore class is for the functionalities that the service layer will call when 
    it wants to add or get or delete things from the backend
*/
class ClientForKeyValueStore {
    public:
        explicit ClientForKeyValueStore(std::shared_ptr<Channel> channel) :stub_(KeyValueStore::NewStub(channel)){}
        void put(std::string key, std::string value); //put data into backend
        std::string get(std::string key); //get information from backend
        void deletekey(std::string key); //delete infromation in backend
    private:
        std::unique_ptr<KeyValueStore::Stub> stub_;
};

/*
    Chirp2Impl class is for the functionalties that would be called from the ClientCommandLine client. 
    It will handle requests from the clientcommandline client and sends a response
*/
class Chirp2Impl final : public KeyValueStore::Service {
    Status deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response); //calls DeleteRequest from the backend.cc and deletes the info
    Status registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response);//Deletes users in backend
    Status chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response); //add chirps into backend storage
    Status follow(ServerContext* context, const FollowRequest* request, FollowReply* response); //add a follower to a specific user to backend storage
    Status read(ServerContext* context, const ReadRequest* request, ReadReply* response); //reads chirps from a user stored in backend
    Status monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter< ::chirp::MonitorReply>* writer); //continuously streaming chirps from all followers all saved in the backend
};

#endif // SERVICE_LAYER_FUNCTIONALITIES_H_