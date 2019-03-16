#ifndef SERVICELAYERVIRTUAL_H
#define SERVICELAYERVIRTUAL_H
#include <iostream>
#include <memory>
#include <string>
#include <queue>
#include <thread>
#include <set>
#include <ctime>
#include <ratio>
#include <chrono>

#include "chirp.grpc.pb.h"

class ServiceLayerVirtual {
 public:
  virtual bool RegisterUser(const std::string &username) = 0;
  virtual bool Chirp(const std::string &username, const std::string &text, const std::string &parentid) = 0;
  virtual bool Follow(const std::string &username, const std::string &toFollow) = 0;
  virtual std::vector <chirp::Chirp> Read(const std::string &chirpid) = 0;
  virtual std::vector <chirp::Chirp> Monitor(const std::string &username) = 0;

};
#endif
