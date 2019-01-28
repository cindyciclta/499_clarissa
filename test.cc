#include <iostream>

#include "gtest/gtest.h"

#include "clientfunctionalities.h"

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