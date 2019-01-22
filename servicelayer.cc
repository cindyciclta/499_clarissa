#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"
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

class ClientForKeyValueStore{
public:
    ClientForKeyValueStore(std::shared_ptr<Channel> channel) :stub_(KeyValueStore::NewStub(channel)){}
    void put(std::string key, std::string value) {
        chirp::PutRequest request;
        request.set_key(key);
        request.set_value(value);

        chirp::PutReply reply;

        ClientContext context;
        Status status = stub_->put(&context, request, &reply);

        if (status.ok()){
            std::cout << "status is ok" << std::endl;
        }else {
            std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
        }
    }
    std::string get(std::string key) {
  
    }
    void Delete(std::string key){
    
    }
private:
    std::unique_ptr<KeyValueStore::Stub> stub_;

};

class Chirp2Impl final : public KeyValueStore::Service {

   
    Status deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response)  {
        return Status::OK;
    }

    Status registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response)  {
        std::cout << "here"<<std::endl;
         ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
        clientKey.put("keytest", request->username());
        return Status::OK;
    }
    Status chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response){
        return Status::OK;
    }
    Status follow(ServerContext* context, const FollowRequest* request, FollowReply* response)  {
        return Status::OK;
    }
    Status read(ServerContext* context, const ReadRequest* request, ReadReply* response) {
        return Status::OK;
    }
    Status monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter< ::chirp::MonitorReply>* writer){
        return Status::OK;
    }
   
};


void RunServer(){
	std::string server_address("0.0.0.0:50002");

	Chirp2Impl service;
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on "<< server_address << std::endl;

	server->Wait();
}

int main(int argc, char** argv){
	RunServer();

	return 0;
}