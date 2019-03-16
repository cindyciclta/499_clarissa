#include <gtest/gtest.h>

#include "servicelayerinstance.h"
#include "keyvaluestoreinstance.h"


TEST(RegisterUser, SimpleTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);
  s_layer.RegisterUser("user1");

  std::string uservalue = kvstore.Get("user1");

  EXPECT_EQ("registered", uservalue);
}

TEST(RegisterUser, MulipleUserTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);
  s_layer.RegisterUser("user1");
  s_layer.RegisterUser("user2");

  std::string uservalue = kvstore.Get("user1");
  std::string seconduservalue = kvstore.Get("user2");
  EXPECT_EQ("registered", uservalue);
  EXPECT_EQ("registered", uservalue);
}

TEST(ChirpToStringTest, ConvertChirptoString) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);

  ChirpObject chirp_;
  chirp_.chirpID_= "1";
  chirp_.parentID_ = "";
  chirp_.text_ = "Testing";
  chirp_.username_ = "user1";

  std::string string_chirp = s_layer.ChirpToString(chirp_);

  EXPECT_EQ("1//Testing/user1", string_chirp);
}

TEST(StringToChirpTest, SimpleTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);

  ChirpObject chirp_;
  chirp_.chirpID_= "1";
  chirp_.parentID_ = "";
  chirp_.text_ = "Testing";
  chirp_.username_ = "user1";

  std::string string_chirp = s_layer.ChirpToString(chirp_);

  ChirpObject converted_chirp = s_layer.StringToChirp(string_chirp);
  EXPECT_EQ(converted_chirp.chirpID_, "1");
  EXPECT_EQ(converted_chirp.parentID_ ,"");
  EXPECT_EQ(converted_chirp.text_, "Testing");
  EXPECT_EQ(converted_chirp.username_, "user1");

}

TEST(ReplyObjectToString, SimpleTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);


  ChirpObject chirp_;
  chirp_.chirpID_= "2";
  chirp_.parentID_ = "5";
  chirp_.text_ = "Testing";
  chirp_.username_ = "user1";
  
  ReplyObject reply_obj;
  reply_obj.username_ = "usermain";
  reply_obj.parentID_ = "5";
  reply_obj.replies_.push_back(chirp_);

  std::string string_reply_obj = s_layer.ReplyObjToString(reply_obj);

  EXPECT_EQ("usermain/5/1/2/5/Testing/user1",string_reply_obj);
}

TEST(ReplyObjectToString, WithMultipleRepliesTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);


  ChirpObject chirp_;
  chirp_.chirpID_= "2";
  chirp_.parentID_ = "5";
  chirp_.text_ = "Testing";
  chirp_.username_ = "user1";
  
  ChirpObject chirp_2;
  chirp_2.chirpID_= "6";
  chirp_2.parentID_ = "3";
  chirp_2.text_ = "Test Text";
  chirp_2.username_ = "user2";

  ReplyObject reply_obj;
  reply_obj.username_ = "usermain";
  reply_obj.parentID_ = "5";
  reply_obj.replies_.push_back(chirp_);
  reply_obj.replies_.push_back(chirp_2);

  std::string string_reply_obj = s_layer.ReplyObjToString(reply_obj);
  EXPECT_EQ("usermain/5/2/2/5/Testing/user1/6/3/Test Text/user2",string_reply_obj);
}

TEST(StringToReplyObj, SimpleTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);

  std::string string_reply_obj = "usermain/5/1/2/5/Testing/user1";
  ReplyObject made_reply_obj = s_layer.StringToReplyObject(string_reply_obj);
  EXPECT_EQ(made_reply_obj.username_, "usermain");
  EXPECT_EQ(made_reply_obj.parentID_, "5");
  EXPECT_EQ(made_reply_obj.replies_.size(), 1);
  EXPECT_EQ(made_reply_obj.replies_.at(0).chirpID_, "2");
}

TEST(StringToReplyObj, MulipleChirpsTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);

  std::string string_reply_obj = "usermain/5/3/2/5/Testing/user1/6/3/Test Text/user2/7/4/Great/user3";
  ReplyObject made_reply_obj = s_layer.StringToReplyObject(string_reply_obj);
  EXPECT_EQ(made_reply_obj.username_, "usermain");
  EXPECT_EQ(made_reply_obj.parentID_, "5");
  EXPECT_EQ(made_reply_obj.replies_.size(), 3);
  EXPECT_EQ(made_reply_obj.replies_.at(1).chirpID_, "6");
  EXPECT_EQ(made_reply_obj.replies_.at(1).text_, "Test Text");
  EXPECT_EQ(made_reply_obj.replies_.at(1).username_, "user2");
  EXPECT_EQ(made_reply_obj.replies_.at(2).chirpID_, "7");
  EXPECT_EQ(made_reply_obj.replies_.at(2).parentID_, "4");
  EXPECT_EQ(made_reply_obj.replies_.at(2).text_, "Great");
  EXPECT_EQ(made_reply_obj.replies_.at(2).username_, "user3");
}

TEST(UserObjToStringTest, SimpleTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);

  ChirpObject chirp_;
  chirp_.chirpID_= "2";
  chirp_.parentID_ = "5";
  chirp_.text_ = "Testing";
  chirp_.username_ = "user1";
  
  UserObject user_obj;
  user_obj.username_ = "userTest";
  user_obj.followers_ = {"user1"};
  user_obj.chirps_ = {chirp_};

  std::string string_user_obj = s_layer.UserObjectToString(user_obj);
  EXPECT_EQ("userTest/1/user1/1/2/5/Testing/user1", string_user_obj);
}

TEST(UserObjToStringTest, ComplexTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);

  ChirpObject chirp_;
  chirp_.chirpID_= "2";
  chirp_.parentID_ = "5";
  chirp_.text_ = "Testing";
  chirp_.username_ = "user1";
  
  ChirpObject chirp_2;
  chirp_2.chirpID_= "6";
  chirp_2.parentID_ = "3";
  chirp_2.text_ = "Test Text";
  chirp_2.username_ = "user2";
  
  UserObject user_obj;
  user_obj.username_ = "userTest";
  user_obj.followers_ = {"user1", "user2"};
  user_obj.chirps_ = {chirp_, chirp_2};

  std::string string_user_obj = s_layer.UserObjectToString(user_obj);
  EXPECT_EQ("userTest/2/user1/user2/2/2/5/Testing/user1/6/3/Test Text/user2", string_user_obj);
}

TEST(StringToUserObject, SimpleTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);
  
  std::string string_user_obj = "userTest/1/user1/1/2/5/Testing/user1";

  UserObject user_obj = s_layer.StringToUserObject(string_user_obj);
  EXPECT_EQ(user_obj.username_, "userTest");
  EXPECT_EQ(user_obj.followers_.size(),1);
  EXPECT_EQ(user_obj.followers_[0], "user1");
  EXPECT_EQ(user_obj.chirps_.size(),1);
  EXPECT_EQ(user_obj.chirps_[0].chirpID_,"2");
}

TEST(StringToUserObject, ComplexTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);
  
  std::string string_user_obj = "userTest/2/user1/user2/2/2/5/Testing/user1/6/3/Test Text/user2";

  UserObject user_obj = s_layer.StringToUserObject(string_user_obj);
  EXPECT_EQ(user_obj.username_, "userTest");
  EXPECT_EQ(user_obj.followers_.size(),2);
  EXPECT_EQ(user_obj.followers_[0], "user1");
  EXPECT_EQ(user_obj.followers_[1], "user2");
  EXPECT_EQ(user_obj.chirps_.size(),2);
  EXPECT_EQ(user_obj.chirps_[0].chirpID_,"2");
  EXPECT_EQ(user_obj.chirps_[0].text_,"Testing");
  EXPECT_EQ(user_obj.chirps_[1].text_,"Test Text");
}

TEST(Chirp, SimpleTest) {
  KeyValueStoreInstance kvstore;
  ServiceLayerInstance s_layer(&kvstore);
  s_layer.RegisterUser("user1");

  std::string uservalue = kvstore.Get("user1");

  EXPECT_EQ("registered", uservalue);

  s_layer.Chirp("user1","testing text", "");
  std::string replyvalue = kvstore.Get("chirp1");
  EXPECT_EQ("1//testing text/user1",replyvalue);
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv); 
  return RUN_ALL_TESTS();
}
