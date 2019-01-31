#include <iostream>

#include "gtest/gtest.h"

#include "clientfunctionalities.h"

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

/*
  This ServiceLayerTest will test if the commandline client can successfully request to register a user to the service layer
*/
TEST_F(ServiceLayerTest, statisOK)
{
  testing::internal::CaptureStdout();
    
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  client.registeruser("cindyclarissa");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ("status is ok\n", output);
}