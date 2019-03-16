#include "clientforkeyvaluestore.h"

void ClientForKeyValueStore::put(const std::string &key, const std::string &value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  ClientContext context;
  Status status = stub_->put(&context, request, &reply);
}
std::vector <std::string> ClientForKeyValueStore::get(const std::string &key) {
  std::vector <std::string> replies;
  chirp::GetRequest request;
  request.set_key(key);

  ClientContext context;
  std::shared_ptr<grpc::ClientReaderWriter <GetRequest, GetReply>> stream(stub_->get(&context));

  stream->Write(request);
  stream->WritesDone();

  chirp::GetReply reply;
  while(stream->Read(&reply)) {
    replies.push_back(reply.value());
  }

  Status status = stream->Finish();

  return replies;
}
void ClientForKeyValueStore::deletekey(const std::string &key) {
  chirp::DeleteRequest request;
  chirp::DeleteReply reply;

  ClientContext context;
  Status status = stub_->deletekey(&context, request, &reply);
}
