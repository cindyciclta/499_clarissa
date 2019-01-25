#ifndef CLIENT_COMMANDLINE_H_
#define CLIENT_COMMANDLINE_H_

#include "clientcommandline.h"


/**
	Implmentation of ClientCommandLine.h
*/
void ClientCommandLine::registeruser(std::string username){
	chirp::RegisterRequest request;
	request.set_username(username);
	chirp::RegisterReply reply;
        
    ClientContext context;
    Status status = stub_->registeruser(&context, request, &reply);

     if (status.ok()){
        std::cout << "status is ok" << std::endl;
    }else {
        std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
    }
}
void ClientCommandLine::chirp(std::string username, std::string text, std::string parent_id){
	//To-Do: Sends a chirp to the service layer
}
void ClientCommandLine::follow(std::string username, std::string to_follow){
	//To-Do: Follows a fellow chirper and sends the appropiate data to the service layer
}
void ClientCommandLine::read(std::string chirp_id){
	//To-Do: Reads a chirp thread
}
void ClientCommandLine::monitor(std::string username){
	//To-Do: Continuiously stream chirps from all followed users. Sends a request to service layer
}
/**
	Main function where the command line tool will be implemented. 
	To-Do: Create a full implmentation of the command line tool
**/
int main(int argc, char** argv){
	ClientCommandLine client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
	client.registeruser("cindyclarissa");

	return 	0;
}

#endif //CLIENT_COMMANDLINE_H_