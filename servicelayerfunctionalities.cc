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
    std::cout << "status is ok" << std::endl;
  } else {
    std::cout << "Problem with put function of service layer"<<std::endl;
  }
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

  if (status.ok()) {
    std::cout << "status is ok from get" << std::endl;
  }

  return replies;
}
void ClientForKeyValueStore::deletekey(const std::string &key) {
 
}
/*
    Implementing Functionalities for ServerForCommandLineClient
*/
ServerForCommandLineClient::ServerForCommandLineClient() {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::vector <std::string> fromget = clientKey.get("chirps_");
  if (fromget.size() != 0) {
    chirps_ = std::stoi(fromget[0]);
  }
}
Status ServerForCommandLineClient::registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response) {
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

Status ServerForCommandLineClient::follow(ServerContext* context, const FollowRequest* request, FollowReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  /* 
    Tested: Able to add followers to user with an empty followers list and if there exist a followers list
  */
        
  std::string value;
  {
    chirp::User user;
    std::vector <std::string> fromget = clientKey.get(request->username());
    if (fromget.size() == 0) {
      return Status::CANCELLED;
    }
    std::string getKey = fromget[0];
    user.ParseFromString(getKey);
    chirp::Followers* f = user.mutable_followers();
    for (int i = 0; i < f->username_size(); i++) {
      if (f->username(i) == request->to_follow()) {
        return Status::CANCELLED;
      }
    }
    f->add_username(request->to_follow());
    user.SerializeToString(&value);
  }

  clientKey.put(request->username(),value);
  return Status::OK;
}

Status ServerForCommandLineClient::chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) {
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  /*
    Tested: Able to add chirps! "chip<id>" is key and message Proto Chirp is value 
  */

  std::string nextChirpID;
  {
    std::lock_guard<std::mutex> lock(mymutex_);
    ++chirps_;
    nextChirpID = std::to_string(chirps_);
    clientKey.put("chirps_", nextChirpID);
  }
  chirp::User user;

  std::vector <std::string> fromget = clientKey.get(request->username());
  if (fromget.size() == 0) {
    return Status::CANCELLED;
  }
  std::string getValue = fromget[0];
  user.ParseFromString(getValue);
  std::string userKey;
  
  //Add new Chirp to User Proto
  std::string value; 
  {
    chirp::Chirp* message = user.add_chirps();
    message->set_username(request->username());
    message->set_text(request->text());
    message->set_id(nextChirpID);  
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
    
  }
  clientKey.put(request->username(),userKey); //Adds Chirp to User proto 

  clientKey.put("chirp"+nextChirpID,value);//Add Chirp to backend
  
  //This is a reply chirp
  if (request->parent_id() != "") { 
    std::string value2; 
    {
      std::vector <std::string> fromget = clientKey.get("reply"+request->parent_id());

      chirp::ChirpReplies replies;
      
      //If not found, use the 'replies' as a new ChirpReplies and set parent_id()
      if (fromget.size() == 0) { 
        replies.set_parent_id(request->parent_id());
      } else { //if found, parse from existing ChirpReplies
        replies.ParseFromString(fromget[0]);
      }
      chirp::Chirp* reply = replies.add_chirps();
      reply->set_username(request->username());
      reply->set_text(request->text());
      reply->set_id(nextChirpID);
      reply->set_parent_id(request->parent_id());
      //TODO: Timestamp
      replies.SerializeToString(&value2);
    }
    clientKey.put("reply"+nextChirpID,value2); //Add ReplyChirp to backend
  }
  
  return Status::OK;
}

Status ServerForCommandLineClient::read(ServerContext* context, const ReadRequest* request, ReadReply* response) {
  /*
    Tested: Able to get all chirps and it's replies and send it back via ReadReply
  */
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  std::vector <std::string> fromget = clientKey.get("chirp"+request->chirp_id());
  if (fromget.size() == 0) {
    return Status::CANCELLED;
  }
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
    if (fromget.size() == 0) {
      continue;
    }
    chirp::ChirpReplies chirpReplies = convertToChirpReplies(fromget[0]);

    for (int i = 0; i < chirpReplies.chirps_size(); i++) {
      myqueue.push(chirpReplies.chirps(i));
      chirp::Chirp* c = response->add_chirps();
      copyChirp(c, chirpReplies.chirps(i));
    }
  }
  return Status::OK;
}

void ServerForCommandLineClient::copyChirp(chirp::Chirp* c, const chirp::Chirp &r) {
  c->set_username(r.username());
  c->set_text(r.text());
  c->set_id(r.id());
  c->set_parent_id(r.parent_id());
  //TODO: TIMESTAMP
}
chirp::Chirp ServerForCommandLineClient::convertToChirp(std::string byte) {
  chirp::Chirp c;
  c.ParseFromString(byte);
  return c;
}
chirp::ChirpReplies ServerForCommandLineClient::convertToChirpReplies(std::string byte) {
  chirp::ChirpReplies replies;
  replies.ParseFromString(byte);
  return replies;
}
chirp::User ServerForCommandLineClient::stringToUser(std::string byte) {
  chirp::User user;
  user.ParseFromString(byte);

  return user;
}

void ServerForCommandLineClient::printall(chirp::User user) {
  std::cout << std::endl;
  std::cout << user.username() <<std::endl;
  std::cout << "size of followers :" << user.followers().username_size() <<std::endl;
  
  for(int i=0; i< user.followers().username_size(); i++) {
    std::cout << "following: "<< user.followers().username(i)<<std::endl;
  }
  std::cout << "size of chirps :" << user.chirps_size() <<std::endl;
    for(int i=0; i< user.chirps_size(); i++) {
    std::cout << "chirps: "<< user.chirps(i).text()<<std::endl;
    std::cout << "reply to "<< user.chirps(i).parent_id() <<std::endl;
  }
  std::cout << std::endl;
}

Status ServerForCommandLineClient::monitor(ServerContext* context, const MonitorRequest* request, 
  ::grpc::ServerWriter< ::chirp::MonitorReply>* writer) {

  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  auto fromget = clientKey.get(request->username());
  
  if (fromget.size() == 0) {
    return Status::CANCELLED;
  }
  std::set<std::string> chirpsent;
  chirp::User user; //main User
  user.ParseFromString(fromget[0]);

  chirp::MonitorReply reply;
  chirp::Followers followers = user.followers();
  while (true) {
    for (int i = 0; i < followers.username_size(); i++) {
      auto allfollowers = clientKey.get(followers.username(i));

      if(allfollowers.size() > 0) {
        chirp::User userFollowers;
        std::string temp = allfollowers[0];
        userFollowers.ParseFromString(temp);
        for(int j = 0; j < userFollowers.chirps_size(); j++) {
          auto it = chirpsent.find(userFollowers.chirps(j).id());

          if (it == chirpsent.end()) {
            chirpsent.insert(userFollowers.chirps(j).id());
            chirp::Chirp* c = reply.mutable_chirp();
            copyChirp(c, userFollowers.chirps(j));
            writer->Write(reply);
            reply.clear_chirp();
          } 
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return Status::OK;
}

