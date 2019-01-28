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
	ClientCommandLine is a class that the Command Line Client will call when 
	users are registering, sending a chirp, following other chirpers, and etc.
*/
class ClientCommandLine{
public:
	ClientCommandLine(std::shared_ptr<Channel> channel) :stub_(KeyValueStore::NewStub(channel)){}
	void registeruser(std::string username);
	void chirp(std::string username, std::string text, std::string parent_id);
	void follow(std::string username, std::string to_follow);
	void read(std::string chirp_id);
	void monitor(std::string username);

private:
	std::unique_ptr<KeyValueStore::Stub> stub_;

};