#include <gtest/gtest.h>

#include "keyvaluestoreinstance.h"

TEST(PutTest, PutOneKey) {
  KeyValueStoreInstance kvstore;
  kvstore.put("user1", "user1object");
  std::string getValue = kvstore.get("user1");
  EXPECT_EQ("user1object", getValue);
}

TEST(GetTest, GetKey) {
  KeyValueStoreInstance kvstore;
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv); 
  return RUN_ALL_TESTS();
}