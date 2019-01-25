#include "servicelayerfunctionalities.h"

/*
    Implementing Functionalities for ClientForKeyValueStore
*/
void  ClientForKeyValueStore::put(std::string key, std::string value) {
    chirp::PutRequest request;
    request.set_key(key);
    request.set_value(value);

    chirp::PutReply reply;

    ClientContext context;
    Status status = stub_->put(&context, request, &reply);

    if (status.ok()){
        std::cout << "status is ok" << std::endl;
    }else {
        std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
     }
}
std::string ClientForKeyValueStore::get(std::string key) {
    //To-Do: Functionality from commandline client that takes a key and call 'get' function from Service Layer
}
void ClientForKeyValueStore::deletekey(std::string key){
    //To-Do: Functionality from commandline client that takes a key and 'deletekey' function from service
}
/*
    Implementing Functionalities for Chirp2Impl
*/

Status Chirp2Impl::deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response)  {
    return Status::OK;
}

Status Chirp2Impl::registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response)  {
    ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
    clientKey.put("keytest", request->username());
    return Status::OK;
}
Status Chirp2Impl::chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response){
    //To-Do:
    return Status::OK;
}
Status Chirp2Impl::follow(ServerContext* context, const FollowRequest* request, FollowReply* response)  {
    //To-Do:
    return Status::OK;
}
Status Chirp2Impl::read(ServerContext* context, const ReadRequest* request, ReadReply* response) {
    //To-Do:
    return Status::OK;
}
Status Chirp2Impl::monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter< ::chirp::MonitorReply>* writer){
    //To-Do:
    return Status::OK;
}