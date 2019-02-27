  #include "keyvaluestoreinstance.h"

  void KeyValueStoreInstance::put(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(mymutex_);
    auto it = data_.find(key);
    if(it != data_.end()) {
      it->second = value;
    } else {
      data_.emplace(key, value);
    }
  }

  std::string KeyValueStoreInstance::get(const std::string &key) {
    std::lock_guard<std::mutex> lock(mymutex_);
    auto it = data_.find(key);
    if (it != data_.end()) {
      return it->second;
    }
    return NULL;
  }
  
  bool KeyValueStoreInstance::deletekey(const std::string &key) {
    auto it = data_.find(key);
    if(it != data_.end()) {
      data_.erase(it);
      return true;
    }
    return false;
  }