#ifndef SERVICE_CLIENTFORKEYVALUESTORE_H
#define SERVICE_CLIENTFORKEYVALUESTORE_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <queue>
#include <ratio>
#include <set>
#include <string>
#include <thread>

#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include "../chirp.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using chirp::Chirp;
using chirp::DeleteReply;
using chirp::DeleteRequest;
using chirp::GetReply;
using chirp::GetRequest;
using chirp::KeyValueStore;
using chirp::PutReply;
using chirp::PutRequest;

using chirp::ChirpReply;
using chirp::ChirpRequest;
using chirp::FollowReply;
using chirp::FollowRequest;
using chirp::MonitorReply;
using chirp::MonitorRequest;
using chirp::ReadReply;
using chirp::ReadRequest;
using chirp::RegisterReply;
using chirp::RegisterRequest;
using chirp::ServiceLayer;

using namespace std::chrono;
using time_stamp = std::chrono::time_point<std::chrono::system_clock,
                                           std::chrono::microseconds>;

/*
    ClientForKeyValueStore class is for the functionalities that the service
   layer will call when it wants to add or get or delete things from the
   backend/key value store. AKA the Client to the backend
   (keyvaluestoreserver.h).
*/
class ClientForKeyValueStore {
 public:
  /*
    Constructor that takes in a shared_ptr of many
    grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials())
    and initalizes the channel to stab_.
  */
  explicit ClientForKeyValueStore(std::shared_ptr<Channel> channel)
      : stub_(KeyValueStore::NewStub(channel)) {}
  /*
    Calls by ServerForCommandLineClient, recieving a string key and a serialized
    proto message value as a string. This function calls the put() in the
    keyvaluestoreserver.h/.cc
  */
  void put(const std::string &key, const std::string &value);
  /*
    Calls by ServerForCommandLineClient, recieving a string key. Returns a
    vector of all the data related to the key in a vector. If the vector has a
    size 0, that means there is no key in the database (keyvaluestore.h/.cc).
  */
  std::vector<std::string> get(const std::string &key);
  /*
    Calls by ServerForCommandLineClient, recieving a string key. Deletes the key
    in the database if exists.
  */
  void deletekey(const std::string &key);

 private:
  /* stub_ for GRPC purposes */
  std::unique_ptr<KeyValueStore::Stub> stub_;
};
#endif /*SERVICE_CLIENTFORKEYVALUESTORE_H*/
