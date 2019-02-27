#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <mutex>
#include <vector>

class KeyValueStoreVirtual {
 public: 
  virtual void put(const std::string &key, const std::string &value) = 0;
  virtual std::string get(const std::string &key) = 0;
  virtual bool deletekey(const std::string &key) = 0;
};