
#include "chirpimpl.h"
/*
    RunServer() runs the backend server and recieves any requests from the Service Layer
*/
void RunServer() {
  std::string server_address("0.0.0.0:50000");

  ChirpImpl service;
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on "<< server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  //testing::InitGoogleTest(&argc, argv); 
  RunServer();

  return 0;
}

