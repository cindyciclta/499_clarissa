#include "clientfunctionalities.h"

void ClientFunctionalities::registeruser(const std::string &username) {
  chirp::RegisterRequest request;
  request.set_username(username);
  chirp::RegisterReply reply;
        
  ClientContext context;
  Status status = stub_->registeruser(&context, request, &reply);

  if (status.ok()) {
    std::cout << "status is ok: ClientFunctionalities" << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
}
void ClientFunctionalities::chirp(const std::string &username, const std::string &text, const std::string &parent_id) {
  chirp::ChirpRequest request;
  request.set_username(username);
  request.set_text(text);
  request.set_parent_id(parent_id);
  //TODO: count chirps and make it as chirp ID
  //TODO: Timestamp
  chirp::ChirpReply reply;
  ClientContext context;
  Status status = stub_->chirp(&context, request, &reply);

  if (status.ok()) {
    std::cout << "status is ok: ClientFunctionalities" << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
  
}
void ClientFunctionalities::follow(const std::string &username, const std::string &to_follow) {
  chirp::FollowRequest request;
  request.set_username(username);
  request.set_to_follow(to_follow);
  chirp::FollowReply reply;
        
  ClientContext context;
  Status status = stub_->follow(&context, request, &reply);

  if (status.ok()) {
    std::cout << "status is ok: ClientFunctionalities" << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
}
void ClientFunctionalities::read(const std::string &chirp_id) {
  //TODO: Reads a chirp thread
}
void ClientFunctionalities::monitor(const std::string &username) {
  //TODO: Continuiously stream chirps from all followed users. Sends a request to service layer
}