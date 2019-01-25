#include "chirpimpl.h"

/**
    Implmentation of chirpimpl.h
*/
Status ChirpImpl::put(ServerContext* context, const PutRequest* request, PutReply* response){
    std::cout << request->key() << std::endl;
    std::cout << request->value() << std::endl;
    User user(request->key());
    // dataUsers.push_back(user);
    // std::cout << dataUsers.size() << std::endl;
	return Status::OK;
}
Status ChirpImpl::get(ServerContext* context, grpc::ServerReaderWriter< GetReply, GetRequest>* stream) {
    //TO-DO: Streaming chirps to user
    std::vector<GetRequest> received_notes;
    GetRequest request;
    while(stream->Read(&request)){
    	for(const GetRequest& r : received_notes){
    		//unsure what to do here
    	}
    	received_notes.push_back(request);
    }
	return Status::OK;
}


Status ChirpImpl::Delete(ServerContext* context, const DeleteRequest* request, DeleteReply* response){
    //To-Do: Deletes info from backend storage
     return Status::OK;
}