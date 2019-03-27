#include "keyvaluestoreinstance.h"

void KeyValueStoreInstance::Put(const std::string &key,
                                const std::string &value) {
  std::lock_guard<std::mutex> lock(mymutex_);
  auto it = data_.find(key);
  if (it != data_.end()) {
    it->second = value;
  } else {
    data_.emplace(key, value);
  }
}

std::vector<std::string> KeyValueStoreInstance::Get(const std::string &key) {
  std::lock_guard<std::mutex> lock(mymutex_);
  std::vector<std::string> respond;
  auto it = data_.find(key);
  if (it != data_.end()) {
    respond.push_back(it->second);
  }
  return respond;
}

bool KeyValueStoreInstance::DeleteKey(const std::string &key) {
  std::lock_guard<std::mutex> lock(mymutex_);
  auto it = data_.find(key);
  if (it != data_.end()) {
    data_.erase(it);
    return true;
  }
  return false;
}

std::string KeyValueStoreInstance::GetFromMap(const std::string &key) {
  auto it = data_.find(key);
  if (it != data_.end()) {
    return it->second;
  }
  return "";
}
