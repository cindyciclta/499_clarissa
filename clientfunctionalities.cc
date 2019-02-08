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
std::multimap<std::string, std::string> ClientFunctionalities::read(const std::string &chirp_id) {
  /* 
    Tested: Able to recieve replied chirps from chirp_id!
  */
  chirp::ReadRequest request;
  request.set_chirp_id(chirp_id);
  chirp::ReadReply reply;
  ClientContext context;
  Status status = stub_->read(&context, request, &reply);

  std::multimap <std::string,std::string> mymap;
  for (int i = 0; i < reply.chirps_size(); i++) {
    chirp::Chirp c = reply.chirps(i);
    mymap.emplace(c.username(),c.text()); 
    //TODO: Add Timestamp with this
  }

  if (status.ok()) {
    std::cout << "status is ok: ClientFunctionalities" << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
  return mymap;
}
void ClientFunctionalities::monitor(const std::string &username) {
  //TODO: Continuiously stream chirps from all followed users. Sends a request to service layer
  chirp::MonitorRequest request;
  request.set_username(username);
  chirp::MonitorReply reply;
  ClientContext context;
  std::unique_ptr <grpc::ClientReader<chirp::MonitorReply> > reader(stub_->monitor(&context, request));
  while(true) {
    if(reader->Read(&reply)) {
      std::cout << "UTLIMATE TEST MONITORR: "<<reply.chirp().username() <<std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  // while(reader->Read(&reply)) {
  //   std::cout << "UTLIMATE TEST MONITORR: "<<reply.chirp().username() <<std::endl;
  // }
  // if (status.ok()) {
  //   std::cout << "status is ok: ClientFunctionalities" << std::endl;
  // } else {
  //   std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  // }
}