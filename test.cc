#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"
#include <gtest/gtest.h>

#include "clientfunctionalities.h"
#include "chirpimpl.h"


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

/* 
  A dummy verision to test chirpimpl class called BackendClientTest
*/
std::unordered_map<std::string, std::string >  data_;
class BackendClientTest {
 public:
  void put(const std::string &key, const std::string &value) {
    addkey(key, value);
  }
  std::string get(const std::string &key) {
    auto it = data_.find(key);
      if(it != data_.end()) {
        return it->second;
      } 
  }
  void chirp(const std::string &key, const std::string &text, const std::string &parent_id) {

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
/*
  ServiceLayerTestFollowers tests that you can register users and add a follower
*/

TEST(ServiceLayerTestFollowers, statisOK) {
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
/*  
  ServiceLayerRegisterUser tests if you are able to add users sucessfully to the map
*/
TEST(ServiceLayerRegisteruser, statisOK) {
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
/*  
  ServiceLayerChrip tests if you are able to add chirps 
*/
TEST(ServiceLayerChrip, statisOK) {
  BackendClientTest client;
  std::string key, value;

  chirp::Username user;
  user.set_username("user1");
  user.SerializeToString(&key);

  chirp::User user2;
  user2.set_username("user2");
  user2.SerializeToString(&value);

  client.put(key, value);
  ASSERT_TRUE(data_.size() != 0);
  EXPECT_EQ(1,data_.size());
  
  client.chirp("user1", "testing chirp", "1");

  std::string test;
  {
    std::string chirpmsg = client.get("chirp1");
    chirp::Chirp c;
    c.ParseFromString(value);
    EXPECT_EQ("testing chirp",c.text());
  }
}


//   This ServiceLayerTest will test if the commandline client can successfully request to register a user to the service layer via GRPC


// TEST(ServiceLayerTest, statisOK)
// {
//   testing::internal::CaptureStdout();
    
//   client.registeruser("user1");
//   client.registeruser("user2");
//   client.registeruser("user3");
//   client.follow("user1", "user2");
//   client.chirp("user1", "testing chirp", "none");
//   client.chirp("user1", "testing chirp222222", "1");
//   client.chirp("user2", "replying to 1", "1");
//   auto chirpthread = client.read("1");
//   auto it = chirpthread.begin();

//   for(const auto& i : chirpthread) {
//     std::cout << i.first << ": "<< i.second << std::endl;
//   }
// }


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv); 
  return RUN_ALL_TESTS();
}