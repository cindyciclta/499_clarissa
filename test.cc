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

std::unordered_map<std::string, std::string >  data_;
class BackendClientTest {
 public:
  void put(std::string key, std::string value) {
    addkey(key, value);
  }
  std::string get(std::string key) {
    auto it = data_.find(key);
      if(it != data_.end()) {
        return it->second;
      } 
  }
 private:
  void addkey(const std::string &key, const std::string &value) {
    auto it = data_.find(key);
    if(it != data_.end()) {
      it->second = value;
    } else {
      data_.emplace(key, value);
    }
  }
};

TEST(ServiceLayerTestFollowers, statisOK)
{
  testing::internal::CaptureStdout();
  
  BackendClientTest client;

  // ClientForKeyValueStore client(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string valueforregisteruser1; 
  {
    chirp::User userValue;
    userValue.set_username("user1"); 
    userValue.SerializeToString(&valueforregisteruser1);
  }

  std::string valueforregisteruser2; 
  {
    chirp::User userValue;
    userValue.set_username("user2"); 
    userValue.SerializeToString(&valueforregisteruser2);
  }

  client.put("user1", valueforregisteruser1);
  auto it = data_.find("user1");
  EXPECT_EQ("user2",it->first);

  client.put("user2", valueforregisteruser2);
  it = data_.find("user2");
  EXPECT_EQ("user2",it->first);
  
  //Add a followe to user1 for testing
  std::string valueforfollow;
  {
    chirp::User user;
    std::string getKey = client.get("user1");
    user.ParseFromString(getKey);

    chirp::Followers* f = user.mutable_followers();
    f->add_username("user2");
    user.SerializeToString(&valueforfollow);
    client.put("user1", valueforfollow);
  }

  //Checks if the value for key: user1 is correct
  std::string test; 
  {
    std::string getKey = client.get("user1");
    chirp::User user;
    user.ParseFromString(getKey);
    EXPECT_EQ(user.username(), "user1");
    EXPECT_EQ(user.followers().username_size(),1);
    EXPECT_EQ(user.followers().username(0),"user2");
  }

}

TEST(ServiceLayerRegisteruser, statisOK)
{
  BackendClientTest client;
  std::string key, value;

  chirp::Username user;
  user.set_username("Cindyclarissa");
  user.SerializeToString(&key);

  chirp::User user2;
  user2.set_username("Cindyclarissa");
  user2.SerializeToString(&value);

  client.put(key, value);
  ASSERT_TRUE(data_.size() != 0);
  EXPECT_EQ(1,data_.size());
  
  std::string key2;
  chirp::Username user3;
  user3.set_username("Cindyclarissa");
  user3.SerializeToString(&key2);
  
  auto it = data_.find(key2);
  EXPECT_EQ(key,it->first);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv); 
  return RUN_ALL_TESTS();
}