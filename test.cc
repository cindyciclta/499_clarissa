#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"
#include <gtest/gtest.h>

#include "clientfunctionalities.h"
#include "chirpimpl.cc"
#include "backend_client_unit_test.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using chirp::RegisterRequest;
using chirp::RegisterReply;
using chirp::ChirpRequest;
using chirp::ChirpReply;
using chirp::FollowRequest;
using chirp::FollowReply;
using chirp::ReadRequest;
using chirp::ReadReply;
using chirp::MonitorRequest;
using chirp::MonitorReply;
using chirp::ServiceLayer;
using chirp::KeyValueStore;

//threadtest1 and 2 is to test multithreading. 
// void threadtest1(){
//   ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
//   std::string username = "user";
//   int i = 0;
//   while(true) {
//     std::string combine = username +  std::to_string(++i);
//     client.registeruser(combine);
//     usleep(50000);
//   }
// }

// void threadtest2(){
//   ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
//   std::string username = "seconduser";
//   int i = 0;
//   while(true) {
//     std::string combine = username +  std::to_string(++i);
//     client.registeruser(combine);
//     usleep(50000);
//   }
// }

/*
  This ServiceLayerTest will test if the commandline client can successfully request to register a user to the service layer
*/
// TEST(ServiceLayerTest, statisOK)
// {
//   testing::internal::CaptureStdout();
    
//   ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
//   client.registeruser("cindyclarissa");
//   std::string output = testing::internal::GetCapturedStdout();
//   EXPECT_EQ("status is ok: ClientFunctionalities\n", output);
// }

TEST(ServiceLayerTestFollowers, statisOK)
{
  testing::internal::CaptureStdout();
    
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  client.registeruser("cindyclarissa");
  client.registeruser("cindyclarissa2");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ("status is ok: ClientFunctionalities\n", output);

  client.follow("cindyclarissa", "cindyclarissa2");

}
std::unordered_map<std::string, std::string >  data_;
class BackendClientTest {
 public:
  void put(std::string key, std::string value) {
    data_.emplace(key, value);
  }
};

// TEST(chirpImplBackend, statisOK)
// {
//   BackendClientTest client;
//   std::string key, value;

//   chirp::Username user;
//   user.set_username("Cindyclarissa");
//   user.SerializeToString(&key);

//   chirp::User user2;
//   user2.set_username("Cindyclarissa");
//   user2.SerializeToString(&value);

//   client.put(key, value);
//   ASSERT_TRUE(data_.size() != 0);
//   EXPECT_EQ(1,data_.size());
  
//   std::string key2;
//   chirp::Username user3;
//   user3.set_username("Cindyclarissa");
//   user3.SerializeToString(&key2);
  
//   auto it = data_.find(key2);
//   EXPECT_EQ(key,it->first);
// }

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv); 
  return RUN_ALL_TESTS();
}