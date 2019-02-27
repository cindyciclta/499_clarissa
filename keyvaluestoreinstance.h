#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <mutex>
#include <vector>

#include "keyvaluestorevirtual.h"

class KeyValueStoreInstance : public KeyValueStoreVirtual {
 public: 
  KeyValueStoreInstance() {};
  void put(const std::string &key, const std::string &value);
  std::string get(const std::string &key);
  bool deletekey(const std::string &key);
 private:
  /* 
    Attempt to store all data in this Map. The key will be std::string of "{username}", "chirp<ID>",
    or "reply<ID>". The values (serialized proto messages) stores user's info (chirp::User), 
    chirps (chirp::Chirp), reply chirps (chirp::Chirp) respectively.
  */
  std::unordered_map<std::string, std::string>  data_;
  /*
    Mutex to safely lock threads/clients from accessing the map, data_, at the same time
  */
  std::mutex mymutex_;
};