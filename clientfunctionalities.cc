#include "clientfunctionalities.h"
/**
    Implmentation of ClientFunctionalities.h
*/
void ClientFunctionalities::registeruser(std::string username){
  chirp::RegisterRequest request;
  request.set_username(username);
  chirp::RegisterReply reply;
        
  ClientContext context;
  Status status = stub_->registeruser(&context, request, &reply);

  if (status.ok()) {
      std::cout << "status is ok" << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
}
void ClientFunctionalities::chirp(std::string username, std::string text, std::string parent_id){
  //TODO: Sends a chirp to the service layer
}
void ClientFunctionalities::follow(std::string username, std::string to_follow){
  //TODO: Follows a fellow chirper and sends the appropiate data to the service layer
}
void ClientFunctionalities::read(std::string chirp_id){
  //TODO: Reads a chirp thread
}
void ClientFunctionalities::monitor(std::string username){
  //TODO: Continuiously stream chirps from all followed users. Sends a request to service layer
}