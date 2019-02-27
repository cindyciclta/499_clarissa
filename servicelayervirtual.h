#include <iostream>
#include <memory>
#include <string>
#include <queue>
#include <thread>
#include <set>
#include <ctime>
#include <ratio>
#include <chrono>

class ServiceLayerVirtual {
 public:
  virtual bool registeruser(const std::string &username) = 0;
  virtual void chirp(const std::string &username, const std::string &text, const std::string &parentID) = 0;
  virtual bool follow(const std::string &username, const std::string &toFollow) = 0;
  virtual std::vector <std::string> read(const std::string &chirpID) = 0;
  virtual std::vector <std::string> monitor(const std::string &username) = 0;
    
};