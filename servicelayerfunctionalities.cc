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
    // std::cout << "status is ok from get" << std::endl;
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
  auto fromget = clientKey.get(request->username());
  if (fromget.size() != 0) {
    return Status::CANCELLED;
  }
  std::string value; 
  {
    chirp::User userValue;
    userValue.set_username(request->username()); 
    userValue.SerializeToString(&value);
    printall(userValue);
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
  //Get TimeStamp
  std::time_t seconds = std::time(nullptr);
  // std::cout << std::asctime(std::localtime(&seconds)) << seconds << " seconds since the Epoch\n";
  int64_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  
  //Get next chirp id, and lock it!
  std::string nextChirpID;
  {
    std::lock_guard<std::mutex> lock(mymutex_);
    ++chirps_;
    nextChirpID = std::to_string(chirps_);
    clientKey.put("chirps_", nextChirpID);
  }
  //Get User
  chirp::User user;
  std::vector <std::string> fromget = clientKey.get(request->username());
  if (fromget.size() == 0) {
    return Status::CANCELLED;
  }
  std::string getValue = fromget[0];
  user.ParseFromString(getValue);
  std::string userKey;
  
  //Add new Chirp to User
  std::string value; 
  {
    chirp::Chirp* message = user.add_chirps();
    message->set_username(request->username());
    message->set_text(request->text());
    message->set_id(nextChirpID);  
    chirp::Timestamp* timestamp = message->mutable_timestamp();
    timestamp->set_seconds(static_cast<int64_t>(seconds));
    timestamp->set_useconds(microseconds_since_epoch);
    message->set_parent_id(request->parent_id());
    message->SerializeToString(&value);
    // printallChirp(*message);
    setChirpReply(message, response);
  }

  user.SerializeToString(&userKey);
  clientKey.put(request->username(),userKey); //Adds Chirp to User proto 
  clientKey.put("chirp"+nextChirpID,value);//Add Chirp to backend
  
  //If its a reply to an existing chirp
  if (request->parent_id() != "") { 
    std::string value2; 
    {
      std::vector <std::string> fromget = clientKey.get("reply"+request->parent_id());

      chirp::ChirpReplies replies;
      
      //If chirp id is not found, use the 'replies' as a new ChirpReplies and set parent_id()
      if (fromget.size() == 0) { 
        std::cout << "setting chirpreplies reply"<< request->parent_id() <<std::endl;
        replies.set_parent_id(request->parent_id());
      } else { //If found, parse from existing ChirpReplies
        std::cout << "found! no need to create chirp replies" <<std::endl;
        replies.ParseFromString(fromget[0]);
      }
      chirp::Chirp* reply = replies.add_chirps();
      reply->set_username(request->username());
      reply->set_text(request->text());
      reply->set_id(nextChirpID);
      reply->set_parent_id(request->parent_id());
      chirp::Timestamp* timestamp = reply->mutable_timestamp();
      timestamp->set_seconds(static_cast<int64_t>(seconds));
      timestamp->set_useconds(microseconds_since_epoch);
      std::cout << "Size of chirp replies is "<< replies.chirps_size() <<std::endl;
      replies.SerializeToString(&value2);
    }
    clientKey.put("reply"+request->parent_id(),value2); //Add reply<ID> to backend
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
  // printallChirp(*mainChirp);
  //DFS
  while(!myqueue.empty()) {
    chirp::Chirp currentChirp = myqueue.front();
    myqueue.pop();
    //Find other chirps that are in reply to this currentChirp
    fromget = clientKey.get("reply"+currentChirp.id());
    if(fromget.size() == 0) {
      std::cout << "reply"<< currentChirp.id() << " is empty! "<< std::endl;
      continue;
    }
    chirp::ChirpReplies chirpReplies = convertToChirpReplies(fromget[0]);
    std:: cout << "chirp id "<< currentChirp.id()<<std::endl;
    std:: cout << "replies "<< chirpReplies.chirps_size()<<std::endl;
    for (int i = 0; i < chirpReplies.chirps_size(); i++) {
      myqueue.push(chirpReplies.chirps(i));
      chirp::Chirp* c = response->add_chirps();
      copyChirp(c, chirpReplies.chirps(i));
    }
  }
  return Status::OK;
}
Status ServerForCommandLineClient::monitor(ServerContext* context, const MonitorRequest* request, 
  ::grpc::ServerWriter< ::chirp::MonitorReply>* writer) {

  std::time_t seconds = std::time(nullptr);
  int64_t seconds_since_epoch= static_cast<int64_t>(seconds);
  int64_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  
  ClientForKeyValueStore clientKey(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  auto fromget = clientKey.get(request->username());
  
  if (fromget.size() == 0) {
    return Status::CANCELLED;
  }
  std::set<std::string> chirpsent;
  chirp::User user; //Main User
  user.ParseFromString(fromget[0]);

  chirp::MonitorReply reply;
  chirp::Followers followers = user.followers();
  //Look through all user's followers, and their chirps. Keep all sent chirps in a set called
  //chirpsent. Keep looking for new chirps with this while loop.
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
void ServerForCommandLineClient::copyChirp(chirp::Chirp* c, const chirp::Chirp &r) {
  c->set_username(r.username());
  c->set_text(r.text());
  c->set_id(r.id());
  c->set_parent_id(r.parent_id());
  chirp::Timestamp* t = c->mutable_timestamp();
  t->set_seconds(r.timestamp().seconds());
  t->set_useconds(r.timestamp().useconds());
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
void ServerForCommandLineClient::printallChirp(chirp::Chirp c) {
  std::cout << std::endl;
  std::cout << c.username() <<std::endl;
  std::cout << c.text() <<std::endl;
  // std::cout << c.timestamp().seconds() << std::endl;
  // std::cout << c.timestamp().useconds() << std::endl;
  std::cout << "ID:" <<c.id() <<std::endl;
  std::cout << "parent id" <<c.parent_id() <<std::endl;
}
void ServerForCommandLineClient::setChirpReply(chirp::Chirp* chirp, chirp::ChirpReply* response) {
  chirp::Chirp* newChirp = response->mutable_chirp();
  newChirp->set_username(chirp->username());
  newChirp->set_text(chirp->text());
  newChirp->set_id(chirp->id());
  newChirp->set_parent_id(chirp->parent_id());
  chirp::Timestamp* timestamp = newChirp->mutable_timestamp();
  timestamp->set_seconds(chirp->timestamp().seconds());
  timestamp->set_useconds(chirp->timestamp().useconds());
}