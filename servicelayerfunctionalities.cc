#include "servicelayerfunctionalities.h"

/*
    Implementing Functionalities for ClientForKeyValueStore
*/
void ClientForKeyValueStore::put(const std::string &key, const std::string &value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  ClientContext context;
  Status status = stub_->put(&context, request, &reply);

  if (status.ok()) {
    // std::cout << "status is ok" << std::endl;
  } else {
    std::cout << "Problem with put function of service layer"<<std::endl;
  }
}
std::vector<std::string> ClientForKeyValueStore::get(const std::string &key) {
  //TODO: Functionality from commandline client that takes a key and call 'get' function from Service Layer
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
  //TODO: multithreading for multitasks
  Status status = stream->Finish();

  if (status.ok()) {
    // std::cout << "status is ok from get" << std::endl;
  }
  else {
    replies.push_back("not found");
  }
  return replies;
}
void ClientForKeyValueStore::deletekey(const std::string &key) {
  //TODO: Functionality from commandline client that takes a key and 'deletekey' function from service
}
/*
    Implementing Functionalities for Chirp2Impl
*/

Status Chirp2Impl::registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  /* 
    Tested: Able to add new users
  */
  std::string value; 
  {
    chirp::User userValue;
    userValue.set_username(request->username()); 
    userValue.SerializeToString(&value);
  }
  clientKey.put(request->username(), value);
  return Status::OK;
}

Status Chirp2Impl::follow(ServerContext* context, const FollowRequest* request, FollowReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  /* 
    Tested: Able to add followers to user with an empty followers list and if there exist a followers list
  */
        
  std::string value;
  {
    chirp::User user;
    std::vector <std::string> fromget = clientKey.get(request->username());
    std::string getKey = fromget[0];
    user.ParseFromString(getKey);
    chirp::Followers* f = user.mutable_followers();
    f->add_username(request->to_follow());
    user.SerializeToString(&value);
  }

  clientKey.put(request->username(),value);
  return Status::OK;
}

Status Chirp2Impl::chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  /*
    Tested: Able to add chirps! "chip<id>" is key and message Proto Chirp is value 
  */
  chirp::User user;

  std::vector <std::string> fromget = clientKey.get(request->username());
  std::string getValue = fromget[0];
  user.ParseFromString(getValue);
  std::string userKey;
  
  //Add new Chirp to User Proto
  std::string value; 
  {
    chirp::Chirp* message = user.add_chirps();
    message->set_username(request->username());
    message->set_text(request->text());
    message->set_id(std::to_string(++chirps_ ) );
    //TODO: Timestamp
    message->set_parent_id(request->parent_id());
    message->SerializeToString(&value);
  }

  user.SerializeToString(&userKey);
  std::string test;
  {
    chirp::User testuser;
    testuser.ParseFromString(userKey);
    printall(testuser);
    // std::cout << "test _____________ "<< testuser.username() << " "<< testuser.chirps_size() <<std::endl;
  }
  clientKey.put(request->username(),userKey); //Adds Chirp to User proto 

  clientKey.put("chirp"+std::to_string(chirps_),value);//Add Chirp to backend
  
  //This is a reply chirp
  if (request->parent_id() != "none") { 
    std::string value2; 
    {
      std::vector <std::string> fromget = clientKey.get("reply"+request->parent_id());
      std::string ifReplyExist = fromget[0];
      chirp::ChirpReplies replies;
      
      //If not found, use the 'replies' as a new ChirpReplies and set parent_id()
      if (ifReplyExist == "not found") { 
        replies.set_parent_id(request->parent_id());
      } else { //if found, parse from existing ChirpReplies
        replies.ParseFromString(ifReplyExist);
      }
      chirp::Chirp* reply = replies.add_chirps();
      reply->set_username(request->username());
      reply->set_text(request->text());
      reply->set_id(std::to_string(chirps_ ) );
      reply->set_parent_id(request->parent_id());
      //TODO: Timestamp
      replies.SerializeToString(&value2);
    }
    clientKey.put("reply"+request->parent_id(),value2); //Add ReplyChirp to backend
  }
  
  return Status::OK;
}

//TODO: Start with first ChipID and call get from that.
//Then get Reply chips to that, and keep going on!
Status Chirp2Impl::read(ServerContext* context, const ReadRequest* request, ReadReply* response) {
  /*
    Tested: Able to get all chirps and it's replies and send it back via ReadReply
  */
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  std::vector <std::string> fromget = clientKey.get("chirp"+request->chirp_id()); //TEST
  std::string mainChirpMsg = fromget[0];

  chirp::Chirp* mainChirp = response->add_chirps();
  mainChirp->ParseFromString(mainChirpMsg);

  std::queue<chirp::Chirp> myqueue;
  myqueue.push(*mainChirp);
  
  //DFS
  while(!myqueue.empty()) {
    chirp::Chirp currentChirp = myqueue.front();
    myqueue.pop();
    //Find other chirps that are in reply to this currentChirp
    fromget = clientKey.get("reply"+currentChirp.id());
    chirp::ChirpReplies chirpReplies = convertToChirpReplies(fromget[0]);

    for (int i = 0; i < chirpReplies.chirps_size(); i++) {
      myqueue.push(chirpReplies.chirps(i));
      chirp::Chirp* c = response->add_chirps();
      copyChirp(c, chirpReplies.chirps(i));
    }
  }
  return Status::OK;
}
void Chirp2Impl::copyChirp(chirp::Chirp* c, const chirp::Chirp &r) {
  c->set_username(r.username());
  c->set_text(r.text());
  c->set_id(r.id());
  c->set_parent_id(r.parent_id());
  //TODO: TIMESTAMP
}
chirp::Chirp Chirp2Impl::convertToChirp(std::string byte) {
  chirp::Chirp c;
  c.ParseFromString(byte);
  return c;
}
chirp::ChirpReplies Chirp2Impl::convertToChirpReplies(std::string byte) {
  chirp::ChirpReplies replies;
  replies.ParseFromString(byte);
  return replies;
}
chirp::User Chirp2Impl::stringToUser(std::string byte) {
  chirp::User user;
  user.ParseFromString(byte);

  return user;
}

void Chirp2Impl::printall(chirp::User user) {
  std::cout << std::endl;
  std::cout << user.username() <<std::endl;
  std::cout << "size of followers :" << user.followers().username_size() <<std::endl;
  for(int i=0; i< user.followers().username_size(); i++) {
    std::cout << "following: "<< user.followers().username(i)<<std::endl;
  }
  std::cout << "size of chirps :" << user.chirps_size() <<std::endl;
    for(int i=0; i< user.chirps_size(); i++) {
    std::cout << "chirps: "<< user.chirps(i).text()<<std::endl;
  }
  std::cout << std::endl;
}
Status Chirp2Impl::monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter< ::chirp::MonitorReply>* writer) {
  //TODO: Takes a request from service layer, continuiously sends data from backend storage 
  std::cout << "HERE?!"<<std::endl;
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  auto fromget = clientKey.get(request->username());

  chirp::MonitorReply reply;
  chirp::User user; //main User
  user.ParseFromString(fromget[0]);
  std::cout << "Main user: " <<user.username() <<std::endl;
  printall(user);
  chirp::Followers followers = user.followers();

  for (int i = 0; i < followers.username_size(); i++) {
    auto allfollowers = clientKey.get(followers.username(i));
    if(allfollowers[0] != "not found") {
      chirp::User userFollowers = stringToUser(allfollowers[i]);
      // printall(userFollowers);
      // std:: cout << "dont tell me there isnt any chips "<< userFollowers.followers().username_size() <<std::endl;
      for(int j = 0; j < userFollowers.chirps_size(); j++) {
        chirp::Chirp* c = reply.mutable_chirp();
        copyChirp(c, userFollowers.chirps(i));
        writer->Write(reply);
        reply.clear_chirp();
      }
    }
  }


  // writer->Writer()
  return Status::OK;
}