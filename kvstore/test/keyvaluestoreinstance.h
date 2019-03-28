#ifndef KVSTORE_TEST_KEYVALUESTOREINSTANCE_H
#define KVSTORE_TEST_KEYVALUESTOREINSTANCE_H
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
/*
  KeyValueStoreInstance class is used for testing purposes and also a
  intermediary for keyvaluestoreserver.cc (this will a private data member in
  keyvaluestoreserver.cc).
*/
class KeyValueStoreInstance {
 public:
  KeyValueStoreInstance(){};  /* Constructor */
  ~KeyValueStoreInstance(){}; /* Deconstructor */
  /*
    Put() takes in a key and value and stores into kvstore
  */
  void Put(const std::string &key, const std::string &value);
  /*
    Get() takes in a key a returns values to that key
  */
  std::vector<std::string> Get(const std::string &key);
  /*
    DeleteKey() takes in a key and returns true or false it was able to delete
    the key.
  */
  bool DeleteKey(const std::string &key);
  /*
  To test the Put() without using Get() to validate, this function returns
  the value from the private variable data_ that stores all data.
  */
  std::string GetFromMap(const std::string &key);

 private:
  /*
    Attempt to store all data in this Map. The key will be std::string of
    "{username}", "chirp<ID>", or "reply<ID>". The values are serialized proto
    messages that stores user's info (chirp::User), chirps (chirp::Chirp), and
    reply chirps (chirp::ChirpReplies) respectively.
  */
  std::unordered_map<std::string, std::string> data_;
  /*
    Mutex to safely lock threads/clients from accessing the map, data_, at the
    same time
  */
  std::mutex mymutex_;
};
#endif /*KVSTORE_TEST_KEYVALUESTOREINSTANCE_H */
