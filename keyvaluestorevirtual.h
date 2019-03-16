#ifndef KEYVALUESTOREVIRTUAL_H
#define KEYVALUESTOREVIRTUAL_H
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <mutex>
#include <vector>

class KeyValueStoreVirtual {
 public: 
  virtual void Put(const std::string &key, const std::string &value) = 0;
  virtual std::vector<std::string> Get(const std::string &key) = 0;
  virtual bool DeleteKey(const std::string &key) = 0;
};
#endif