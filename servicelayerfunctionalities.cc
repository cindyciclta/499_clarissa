#include "servicelayerfunctionalities.h"

/*
    Implementing Functionalities for ClientForKeyValueStore
*/

void ClientForKeyValueStore::Put(const std::string &key, const std::string &value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  ClientContext context;
  Status status = stub_->put(&context, request, &reply);
}

std::vector <std::string> ClientForKeyValueStore::Get(const std::string &key) {
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

void ClientForKeyValueStore::DeleteKey(const std::string &key) {
  chirp::DeleteRequest request;
  chirp::DeleteReply reply;

  ClientContext context;
  Status status = stub_->deletekey(&context, request, &reply);
}
/*
    Implementing Functionalities for ServerForCommandLineClient
*/
ServerForCommandLineClient::ServerForCommandLineClient() {
  ClientForKeyValueStore client_key(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::vector <std::string> vector_from_get_func = client_key.Get("chirps_");
  if (vector_from_get_func.size() == 0) {
    client_key.Put("chirps_", std::to_string(0));
  }
}

Status ServerForCommandLineClient::RegisterUser(ServerContext* context, const RegisterRequest* request, RegisterReply* response) {
  ClientForKeyValueStore client_key(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  /* 
    Tested: Able to add new users
  */
  auto vector_from_get_func = client_key.Get(request->username());
  if (vector_from_get_func.size() != 0) {
    return Status::CANCELLED;
  }
  std::string value; 
  {
    chirp::User userValue;
    userValue.set_username(request->username()); 
    userValue.SerializeToString(&value);
  }
  client_key.Put(request->username(), value);
  return Status::OK;
}

Status ServerForCommandLineClient::Follow(ServerContext* context, const FollowRequest* request, FollowReply* response) {
  /* 
    Tested: Able to add followers to user with an empty followers list and if there exist a followers list
  */
  ClientForKeyValueStore client_key(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  if (request->username() == request->to_follow()) {
    return Status::CANCELLED;
  }

  std::string user_object_to_serialize;
  {
    chirp::User user_proto;
    std::vector <std::string> vector_from_get_func = client_key.Get(request->username());
    if (vector_from_get_func.size() == 0) {
      return Status::CANCELLED;
    }
    std::string user_proto_string = vector_from_get_func[0];
    user_proto.ParseFromString(user_proto_string);
    chirp::Followers* new_follower_proto= user_proto.mutable_followers();
    for (int i = 0; i < new_follower_proto->username_size(); i++) {
      if (new_follower_proto->username(i) == request->to_follow()) {
        return Status::CANCELLED;
      }
    }
    new_follower_proto->add_username(request->to_follow());
    user_proto.SerializeToString(&user_object_to_serialize);
  }

  client_key.Put(request->username(),user_object_to_serialize);
  return Status::OK;
}

Status ServerForCommandLineClient::Chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) {
  ClientForKeyValueStore client_key(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  /*
    Tested: Able to add chirps! "chip<id>" is key and message Proto Chirp is value 
  */
  /* Get TimeStamp */
  std::time_t seconds = std::time(nullptr);
  int64_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  
  /* Get next chirp id, and lock it! */
  std::string next_chirp_id;
  {
    std::lock_guard<std::mutex> lock(mymutex_);
    auto vector_from_get_func = client_key.Get("chirps_");
    if (vector_from_get_func.size() == 0) {
      next_chirp_id = std::to_string(0);
    } else {
      int currchirpid = std::stoi(vector_from_get_func[0]);
      currchirpid++;
      next_chirp_id = std::to_string(currchirpid);
    }
    client_key.Put("chirps_", next_chirp_id);
  }
  /* Get User */
  chirp::User user_proto;
  std::vector <std::string> vector_from_get_func = client_key.Get(request->username());
  if (vector_from_get_func.size() == 0) {
    return Status::CANCELLED;
  }
  std::string GetValue = vector_from_get_func[0];
  user_proto.ParseFromString(GetValue);
  std::string userKey;
  
  /* Add new Chirp to chirp::User */
  std::string chirp_proto_to_serialize; 
  {
    chirp::Chirp* message = user_proto.add_chirps();
    message->set_username(request->username());
    message->set_text(request->text());
    message->set_id(next_chirp_id);  
    chirp::Timestamp* timestamp = message->mutable_timestamp();
    timestamp->set_seconds(static_cast<int64_t>(seconds));
    timestamp->set_useconds(microseconds_since_epoch);
    message->set_parent_id(request->parent_id());
    message->SerializeToString(&chirp_proto_to_serialize);
    
    SetChirpReply(message, response);
  }

  user_proto.SerializeToString(&userKey);
  client_key.Put(request->username(),userKey); //Add Chirp to chirp::User
  client_key.Put("chirp"+next_chirp_id, chirp_proto_to_serialize);//Add chirp::Chirp to database
  
  /* If its a reply to an existing chirp */
  if (request->parent_id() != "") { 
    std::string chirp_reply_proto_to_serialize; 
    {
      std::vector <std::string> vector_from_get_func = client_key.Get("reply"+request->parent_id());

      chirp::ChirpReplies replies;
      
      /* If chirp id is not found, use the 'replies' as a new ChirpReplies and set parent_id()*/
      if (vector_from_get_func.size() == 0) { 
        replies.set_parent_id(request->parent_id());
      } else { /* If found, parse from existing ChirpReplies */
        replies.ParseFromString(vector_from_get_func[0]);
      }
      chirp::Chirp* reply = replies.add_chirps();
      reply->set_username(request->username());
      reply->set_text(request->text());
      reply->set_id(next_chirp_id);
      reply->set_parent_id(request->parent_id());
      chirp::Timestamp* timestamp = reply->mutable_timestamp();
      timestamp->set_seconds(static_cast<int64_t>(seconds));
      timestamp->set_useconds(microseconds_since_epoch);
      replies.SerializeToString(&chirp_reply_proto_to_serialize);
    }
    client_key.Put("reply"+request->parent_id(), chirp_reply_proto_to_serialize); /* Add reply<ID> to backend */
  }
  
  return Status::OK;
}

Status ServerForCommandLineClient::Read(ServerContext* context, const ReadRequest* request, ReadReply* response) {
  /*
    Tested: Able to Get all chirps and it's replies and send it back via ReadReply
  */
  ClientForKeyValueStore client_key(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));

  std::vector <std::string> vector_from_get_func = client_key.Get("chirp"+request->chirp_id());
  std::vector <chirp::Chirp> chirps_to_send; //For sorting by timestamp
  if (vector_from_get_func.size() == 0) {
    return Status::CANCELLED;
  }
  std::string main_chirp_msg = vector_from_get_func[0];

  chirp::Chirp* main_chirp = response->add_chirps();
  main_chirp->ParseFromString(main_chirp_msg);
  chirps_to_send.push_back(*main_chirp);

  std::queue<chirp::Chirp> myqueue;
  myqueue.push(*main_chirp);
  
  /* Running DFS to Get threads related to chirp<id> */
  while(!myqueue.empty()) {
    chirp::Chirp current_chirp_in_DFS = myqueue.front();
    myqueue.pop();
    
    /* Find other chirps that are in reply to this current_chirp_in_DFS */
    vector_from_get_func = client_key.Get("reply"+current_chirp_in_DFS.id());
    if(vector_from_get_func.size() == 0) {
      continue;
    }

    chirp::ChirpReplies chirp_replies = ConvertToChirpReplies(vector_from_get_func[0]);
    for (int i = 0; i < chirp_replies.chirps_size(); i++) {
      myqueue.push(chirp_replies.chirps(i));
      chirps_to_send.push_back(chirp_replies.chirps(i));
    }
  }
  /* Sort by timestamp */
  std::sort(chirps_to_send.begin(), chirps_to_send.end(), [](chirp::Chirp &a, chirp::Chirp &b){
    return a.timestamp().useconds() < b.timestamp().useconds();
  });
  for (const auto &i :chirps_to_send) {
    if (i.id() == main_chirp->id()) {
      continue;
    }
    chirp::Chirp* c = response->add_chirps();
    CopyChirp(c, i);
  }
  return Status::OK;
}

Status ServerForCommandLineClient::Monitor(ServerContext* context, const MonitorRequest* request, 
  ::grpc::ServerWriter< ::chirp::MonitorReply>* writer) {
  /* Get Current TimeStamp */
  std::time_t seconds = std::time(nullptr);
  int64_t seconds_since_epoch = static_cast<int64_t>(seconds);
  int64_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  
  ClientForKeyValueStore client_key(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  
  auto vector_from_get_func = client_key.Get(request->username());
  if (vector_from_get_func.size() == 0) {
    return Status::CANCELLED;
  }

  std::set<std::string> chirp_to_send;
  chirp::User user_proto; //Main User
  user_proto.ParseFromString(vector_from_get_func[0]);

  chirp::MonitorReply reply;
  chirp::Followers followers = user_proto.followers();
  /*
    Continuously look through all user's followers, and their chirps. Keep all sent chirps in a set called
    chirp_to_send. Keep looking for new chirps with this while loop.
  */
  while (true) {
    if (context->IsCancelled()) {
      break;
    }
    for (int i = 0; i < followers.username_size(); i++) {
      auto all_curr_user_followers = client_key.Get(followers.username(i));

      if(all_curr_user_followers.size() > 0) {
        chirp::User users_followers;
        std::string temp = all_curr_user_followers[0];
        users_followers.ParseFromString(temp);

        for(int j = 0; j < users_followers.chirps_size(); j++) {
          auto it = chirp_to_send.find(users_followers.chirps(j).id());

          if (it == chirp_to_send.end()) {
            if (users_followers.chirps(j).timestamp().useconds() > microseconds_since_epoch) {
              chirp_to_send.insert(users_followers.chirps(j).id());

              chirp::Chirp* c = reply.mutable_chirp();
              CopyChirp(c, users_followers.chirps(j));
              writer->Write(reply);
              reply.clear_chirp();
            }
          } 
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return Status::OK;
}

void ServerForCommandLineClient::CopyChirp(chirp::Chirp* chirp, const chirp::Chirp &reply_chirp) {
  chirp->set_username(reply_chirp.username());
  chirp->set_text(reply_chirp.text());
  chirp->set_id(reply_chirp.id());
  chirp->set_parent_id(reply_chirp.parent_id());
  chirp::Timestamp* t = chirp->mutable_timestamp();
  t->set_seconds(reply_chirp.timestamp().seconds());
  t->set_useconds(reply_chirp.timestamp().useconds());
}

chirp::Chirp ServerForCommandLineClient::ConvertToChirp(std::string byte) {
  chirp::Chirp chirp;
  chirp.ParseFromString(byte);
  return chirp;
}

chirp::ChirpReplies ServerForCommandLineClient::ConvertToChirpReplies(std::string byte) {
  chirp::ChirpReplies replies;
  replies.ParseFromString(byte);
  return replies;
}

chirp::User ServerForCommandLineClient::StringToUser(std::string byte) {
  chirp::User user;
  user.ParseFromString(byte);

  return user;
}

void ServerForCommandLineClient::SetChirpReply(chirp::Chirp* chirp, chirp::ChirpReply* response) {
  chirp::Chirp* new_chirp = response->mutable_chirp();
  new_chirp->set_username(chirp->username());
  new_chirp->set_text(chirp->text());
  new_chirp->set_id(chirp->id());
  new_chirp->set_parent_id(chirp->parent_id());
  chirp::Timestamp* timestamp = new_chirp->mutable_timestamp();
  timestamp->set_seconds(chirp->timestamp().seconds());
  timestamp->set_useconds(chirp->timestamp().useconds());
}