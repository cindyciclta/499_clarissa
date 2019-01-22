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

class Client{
public:
	Client(std::shared_ptr<Channel> channel) :stub_(KeyValueStore::NewStub(channel)){}
	void registeruser(std::string username){
		chirp::RegisterRequest request;
		request.set_username(username);
		chirp::RegisterReply reply;
		// std::cout << "huh"<<std::endl;
        
        ClientContext context;
        Status status = stub_->registeruser(&context, request, &reply);

        if (status.ok()){
            std::cout << "status is ok" << std::endl;
        }else {
            std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
        }
	}
	void chirp(std::string username, std::string text, std::string parent_id){
		
	}
	void follow(std::string username, std::string to_follow){
		
	}
	void read(std::string chirp_id){
		
	}
	void monitor(std::string username){
		
	}

private:
	std::unique_ptr<KeyValueStore::Stub> stub_;

};
int main(int argc, char** argv){

	Client client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
	client.registeruser("cindyclarissa");

	return 0;
}