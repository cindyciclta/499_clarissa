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
    KeyValueStoreServer is to handle request from Service Layer and submit a response back
**/
class KeyValueStoreServer final : public KeyValueStore::Service {
 public:
  /*
    A client, ServerContext, calls this put() and sends a PutRequest & PutReply.
    Put() takes the request from the client and inputs key and value into the map, data_;
    PutReply is not set to anything because success/failure will be signed via GRPC status
  */
  Status Put(ServerContext* context, const PutRequest* request, PutReply* response); 
  /*
    A client, ServerContext, calls this get() and sends a grpc::ServerReaderWriter stream. 
    This stream will recieve GetRequest sent by the client, retreives the data in data_, 
    then returns a GetReply to the client.
  */
  Status Get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream);
  /*
    A client, ServerContext, calls this deletekey() and sends in DeleteRequest and DeleteReply.
    deletekey() processes the request by deleting a key from the map, data_. DeleteReply is not set 
    to anything because success/failure will be signed via GRPC status.
  */
  Status DeleteKey(ServerContext* context, const DeleteRequest* request, DeleteReply* response);  
 private:
  /* 
    Attempt to store all data in this Map. The key will be std::string of "{username}", "chirp<ID>",
    or "reply<ID>". The values (serialized proto messages) stores user's info (chirp::User), 
    chirps (chirp::Chirp), reply chirps (chirp::Chirp) respectively.
  */
  std::unordered_map<std::string, std::string> data_;
  /*
    Mutex to safely lock threads/clients from accessing the map, data_, at the same time
  */
  std::mutex mymutex_;
  /* 
    Helper function for put() function. It will replace the value with an existing key,
    or put a new key in.
  */
  void AddKey(const std::string &key, const std::string &value);
  bool DeleteKeyHelper(const std::string &key);
};

#endif //CHIRP_IMPL_H