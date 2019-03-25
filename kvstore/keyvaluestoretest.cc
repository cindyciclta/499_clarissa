#include <thread>

#include <gtest/gtest.h>

#include "keyvaluestoreinstance.h"
/*
  Test Put() into kvstore
*/
TEST(PutTest, PutOneKey) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  auto getValue = kvstore.GetFromMap("user1");
  EXPECT_EQ("user1object", getValue);
}
/*
  Test Put(): Multiple keys
*/
TEST(PutTest, MultipleKeys) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  kvstore.Put("user2", "user2object");
  auto getValue = kvstore.GetFromMap("user1");
  EXPECT_EQ("user1object", getValue);
  getValue = kvstore.GetFromMap("user2");
  EXPECT_EQ("user2object", getValue);
}

/*
  Test Put(): Replacing existing key
*/
TEST(PutTest, ExistingKey) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  kvstore.Put("user1", "user2object");
  auto getValue = kvstore.GetFromMap("user1");
  EXPECT_EQ("user2object", getValue);
}

/*
  Test Get() into kvstore
*/

TEST(GetTest, PutOneKey) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  auto getValue = kvstore.Get("user1");
  EXPECT_EQ("user1object", getValue[0]);
}
/*
  Test Put(): Multiple keys
*/
TEST(GetTest, MultipleKeys) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  kvstore.Put("user2", "user2object");
  auto getValue = kvstore.Get("user1");
  EXPECT_EQ("user1object", getValue[0]);
  getValue = kvstore.Get("user2");
  EXPECT_EQ("user2object", getValue[0]);
}

/*
  Test Put(): Replacing existing key
*/
TEST(GetTest, ExistingKey) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  kvstore.Put("user1", "user2object");
  auto getValue = kvstore.Get("user1");
  EXPECT_EQ("user2object", getValue[0]);
}
/*
  Test Delete()
*/
TEST(DeleteTest, SimpleTest) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  kvstore.Put("user1", "user2object");
  auto getValue = kvstore.DeleteKey("user1");
  EXPECT_EQ(true, getValue);

  auto getValue2 = kvstore.Get("user1");
  EXPECT_EQ(0, getValue2.size());
}
/*
  Test Delete() twice
*/
TEST(DeleteTest, DuplicateTest) {
  KeyValueStoreInstance kvstore;
  kvstore.Put("user1", "user1object");
  kvstore.Put("user1", "user2object");
  auto getValue = kvstore.DeleteKey("user1");
  EXPECT_EQ(true, getValue);

  getValue = kvstore.DeleteKey("user1");
  EXPECT_EQ(false, getValue);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
