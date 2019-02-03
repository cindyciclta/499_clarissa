#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"

#include "chirpimpl.h"
#include "servicelayerfunctionalities.h"

class BackendClient {
  void put(std::string key, std::string value) {
    ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
    clientKey.put(key, value);
    // return Status::OK
  }
  // std::string get(std::string key) {
        
  // }
  // std::string delete(std::string key) {
        
  // }
};