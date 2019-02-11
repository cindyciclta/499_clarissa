#include "clientfunctionalities.h"

void ClientFunctionalities::registeruser(const std::string &username) {
  if (username == "") {
    return;
  }
  chirp::RegisterRequest request;
  request.set_username(username);
  chirp::RegisterReply reply;
        
  ClientContext context;
  Status status = stub_->registeruser(&context, request, &reply);

  if (status.ok()) {
    std::cout << "Registered user: "<< username << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
}
void ClientFunctionalities::chirp(const std::string &username, const std::string &text, const std::string &parent_id) {
  chirp::ChirpRequest request;
  request.set_username(username);
  request.set_text(text);
  request.set_parent_id(parent_id);
  
  chirp::ChirpReply reply;
  ClientContext context;
  Status status = stub_->chirp(&context, request, &reply);
  std::cout << "test this bitch "<< static_cast<int64_t>(reply.chirp().timestamp().seconds())<<std::endl;
  if (status.ok()) {
    std::cout << "Successfully chirped!" << std::endl;
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
    std::cout << "Sucessfully followed: "<< to_follow << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }
}
void ClientFunctionalities::read(const std::string &chirp_id) {
  /* 
    Tested: Able to recieve replied chirps from chirp_id!
  */
  chirp::ReadRequest request;
  request.set_chirp_id(chirp_id);
  chirp::ReadReply reply;
  ClientContext context;
  Status status = stub_->read(&context, request, &reply);

  std::multimap <std::string,std::string> mymap;
  std::cout << "size: "<< reply.chirps_size() <<std::endl;
  for (int i = 0; i < reply.chirps_size(); i++) {
    chirp::Chirp c = reply.chirps(i);
    std::time_t seconds = c.timestamp().seconds();
    // std::asctime(std::localtime(&seconds));
    std::cout << "["<<c.username() << " " <<std::asctime(std::localtime(&seconds)) << "]: "<< c.text() << std::endl;
  }

  if (status.ok()) {
    // std::cout << "Successfully read all chirps with thread chirp"<< chirp_id << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
  }

}
void ClientFunctionalities::monitor(const std::string &username) {
  chirp::MonitorRequest request;
  request.set_username(username);
  chirp::MonitorReply reply;
  ClientContext context;
  std::unique_ptr <grpc::ClientReader<chirp::MonitorReply> > reader(stub_->monitor(&context, request));
  while(true) {
    if(reader->Read(&reply)) {
      std::cout << "["<<reply.chirp().username() << "]: "<< reply.chirp().text()<<std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

}