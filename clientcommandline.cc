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
  client.registeruser("user2");
  client.registeruser("user3");
  // // client.chirp("cindyclarissa", "Testing 123","1");
  client.follow("user1", "user2");
  client.follow("user1", "user3");
  client.follow("user1", "user4");
  // client.follow("cindyclarissa", "cindyclarissa3");
  return  0;
}

