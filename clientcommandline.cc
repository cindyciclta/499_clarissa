#include <thread>
#include <unistd.h>

#include "clientfunctionalities.h"

/**
    Main function where the command line tool will be implemented. 
    TODO: Create a full implmentation of the command line tool
**/

int main(int argc, char** argv) {
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  
  client.registeruser("user1");
  std::cout <<"went here "<<std::endl;
  client.registeruser("user2");
  client.registeruser("user3");
  client.follow("user1", "user2");
  client.chirp("user1", "testing chirp", "none");
  client.chirp("user1", "testing chirp222222", "1");
  client.chirp("user2", "replying to 1", "1");
  auto chirpthread = client.read("1");

  for(const auto& i : chirpthread) {
    std::cout << i.first << ": "<< i.second << std::endl;
  }
  return  0;
}

