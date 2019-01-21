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
	void put(std::string key, std::string value) {

	}
    std::string get(std::string key) {
  
    }
    void Delete(std::string key){
    
    }
private:
	std::unique_ptr<KeyValueStore::Stub> stub_;

};
int main(int argc, char** argv){

	Client client(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
	// std::string reply = client.get(1);
	std::cout << "received: "<<std::endl;

	return 0;
}