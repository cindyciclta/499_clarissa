#ifndef KVSTORE_KEYVALUESTORESERVER_H
#define KVSTORE_KEYVALUESTORESERVER_H

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "../chirp.grpc.pb.h"

#include "test/keyvaluestoreinstance.h"

using chirp::DeleteReply;
using chirp::DeleteRequest;
using chirp::GetReply;
using chirp::GetRequest;
using chirp::KeyValueStore;
using chirp::PutReply;
using chirp::PutRequest;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using std::vector;
/**
    KeyValueStoreServer aka kvstore is to handle request from Service Layer and
    submit a response back. This class acts as the methods to store all data.
**/
class KeyValueStoreServer final : public KeyValueStore::Service {
 public:
  /*
    A client, ServerContext, calls this put() and sends a PutRequest & PutReply
    with. Put() takes the request from the client and inputs key and value into
    the map, data_; PutReply is not set to anything because success/failure will
    be signed via GRPC status
  */
  Status put(ServerContext *context, const PutRequest *request,
             PutReply *response);
  /*
    A client, ServerContext, calls this get() and sends a
    grpc::ServerReaderWriter stream. This stream will recieve GetRequest sent by
    the client, retreives the data in data_, then returns a GetReply to the
    client.
  */
  Status get(ServerContext *context,
             grpc::ServerReaderWriter<GetReply, GetRequest> *stream);
  /*
    A client, ServerContext, calls this deletekey() and sends in DeleteRequest
    and DeleteReply. deletekey() processes the request by deleting a key from
    the map, data_. DeleteReply is not set to anything because success/failure
    will be signed via GRPC status.
  */
  Status deletekey(ServerContext *context, const DeleteRequest *request,
                   DeleteReply *response);

 private:
  /*
    kvstore_ is a class that is an intermediary for this class. Ihis object will
    store all the data. The key will be std::string of
    "{username}", "chirp<ID>", or "reply<ID>". The values are serialized proto
    messages that stores user's info (chirp::User), chirps (chirp::Chirp), and
    reply chirps (chirp::ChirpReplies) respectively.
  */
  KeyValueStoreInstance kvstore_;
};

#endif /* KVSTORE_KEYVALUESTORESERVER_H */
