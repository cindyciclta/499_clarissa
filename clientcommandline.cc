#include <thread>
#include <unistd.h>

#include "clientfunctionalities.h"

/**
    Main function where the command line tool will be implemented. 
    TODO: Create a full implmentation of the command line tool
**/

int main(int argc, char** argv) {
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  client.registeruser("cindyclarissa");
  client.registeruser("cindyclarissa2");
  client.registeruser("cindyclarissa3");
  client.chirp("cindyclarissa", "Testing 123","1");
  client.follow("cindyclarissa", "cindyclarissa2");
  return  0;
}

