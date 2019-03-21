#ifndef KEYVALUESTOREINSTANCE_H
#define KEYVALUESTOREINSTANCE_H
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
/* KeyValueStoreInstance class is solely for testing only. This is testing the
   class in keyvaluestoreserver.cc
*/
class KeyValueStoreInstance {
public:
  KeyValueStoreInstance(){};  /* constructor */
  ~KeyValueStoreInstance(){}; /* deconstructor */
  /* Put() takes in a key and value and stores into kvstore */
  void Put(const std::string &key, const std::string &value);
  /* Get() takes in a key a returns values to that key */
  std::vector<std::string> Get(const std::string &key);
  /* DeleteKey() takes in a key and returns true or false it was able to delete
   * the key. */
  bool DeleteKey(const std::string &key);
  /* To test the Put() without using Get() to validate, this function returns
     the value from the private variable data_ that stores all data.
  */
  std::string GetFromMap(const std::string &key);

private:
  /*
    Attempt to store all data in this Map. The key will be std::string of
    "<username>", "chirp<ID>", or "reply<ID>". The values are serialized proto
    messages: user's info (chirp::User), chirps (chirp::Chirp), reply
    chirps (chirp::Chirp) respectively.
  */
  std::unordered_map<std::string, std::string> data_;
  /*
    Mutex to safely lock threads/clients from accessing the map, data_, at the
    same time
  */
  std::mutex mymutex_;
};
#endif
