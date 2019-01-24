#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"
#include "User.h"
#include <unordered_map>



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

    //testing implementation with dummy values
	Status put(ServerContext* context, const PutRequest* request, PutReply* response) override{
        std::cout << request->key() << std::endl;
        std::cout << request->value() << std::endl;
        User user(request->key());

        // dataUsers.push_back(user);
        // std::cout << dataUsers.size() << std::endl;
		return Status::OK;
	}
    Status get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
    	//TO-DO
        std::vector<GetRequest> received_notes;
    	GetRequest request;
    	while(stream->Read(&request)){
    		for(const GetRequest& r : received_notes){
    			//unsure what to do here
    		}
    		received_notes.push_back(request);
    	}

    	return Status::OK;
    }


    Status Delete(ServerContext* context, const DeleteRequest* request, DeleteReply* response){
    	//To-Do
        return Status::OK;
    }

    //DATA SAVED HERE
    std::unordered_map<std::string,User> data(); 
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

