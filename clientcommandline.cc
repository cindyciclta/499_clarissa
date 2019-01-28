#include <thread>
#include <unistd.h>

#include "clientfunctionalities.h"

/**
    Main function where the command line tool will be implemented. 
    TODO: Create a full implmentation of the command line tool
**/
//threadtest1 and 2 is to test multithreading. 
void threadtest1(){
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  std::string username = "user";
  int i = 0;
  while(true) {
    std::string combine = username +  std::to_string(++i);
    client.registeruser(combine);
    usleep(50000);
  }
}

void threadtest2(){
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  std::string username = "seconduser";
  int i = 0;
  while(true) {
    std::string combine = username +  std::to_string(++i);
    client.registeruser(combine);
    usleep(50000);
  }
}

int main(int argc, char** argv) {
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  client.registeruser("cindyclarissa");
  // std::thread first (threadtest1);
  // std::thread second (threadtest2);
  return  0;
}

