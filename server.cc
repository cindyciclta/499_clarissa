#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chirp::KeyValueStore;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::PutReply;
using chirp::PutRequest;
using chirp::GetRequest;
using chirp::DeleteRequest;
using chirp::DeleteReply;
class ChirpImpl final : public KeyValueStore::Service {
	Status put(ServerContext* context, const PutRequest* request, PutReply* response) {
		return Status::OK;
	}
    Status get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
    	return Status::OK;
    }
    Status Delete(ServerContext* context, const DeleteRequest* request, DeleteReply* response){
    	return Status::OK;
    }
	//Do I make 3 Status? for put, get, delete requests?
};

void RunServer(){
	std::string server_address("0.0.0.0:50000");
	ChirpImpl service;
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