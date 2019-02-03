#ifndef CHIRP_IMPL_H
#define CHIRP_IMPL_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <mutex>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "chirp.grpc.pb.h"

using std::vector;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chirp::KeyValueStore;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::PutReply;
using chirp::PutRequest;
using chirp::GetRequest;
using chirp::DeleteRequest;
using chirp::DeleteReply;
/**
    ChirpImpl is to handle request from Service Layer and submit a response back
**/
class ChirpImpl final : public KeyValueStore::Service {
 public:
  //handles request for put from the service layer
  Status put(ServerContext* context, const PutRequest* request, PutReply* response); 
  //handles request for monitor from service layer
  Status get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream);
  //handles delete request from the service layer
  Status deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response); 
 private:
  //Attempt to store all data in this Map. Key is serialized username or ID, values (serialized) stores user's info, or chirps, respectively.
  //TODO: Will need to implement threadsafe functionalities with this
  std::unordered_map<std::string, std::string >  data_;
  // std::vector <std::vector <std::string> > data_;
  //mutex to safely lock threads from accessing data_ at the same time
  std::mutex mymutex_;
  void addkey(const std::string &key, const std::string &value);
};

#endif //CHIRP_IMPL_H