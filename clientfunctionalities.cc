#include "clientfunctionalities.h"

void ClientFunctionalities::registeruser(const std::string &username) {
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
void ClientFunctionalities::chirp(const std::string &username, const std::string &text, const std::string &parent_id) {
  //TODO: Sends a chirp to the service layer
}
void ClientFunctionalities::follow(const std::string &username, const std::string &to_follow) {
  //TODO: Follows a fellow chirper and sends the appropiate data to the service layer
}
void ClientFunctionalities::read(const std::string &chirp_id) {
  //TODO: Reads a chirp thread
}
void ClientFunctionalities::monitor(const std::string &username) {
  //TODO: Continuiously stream chirps from all followed users. Sends a request to service layer
}