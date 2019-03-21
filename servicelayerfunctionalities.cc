#include "servicelayerfunctionalities.h"

/*
    Implementing Functionalities for ClientForKeyValueStore
*/
void ClientForKeyValueStore::put(const std::string &key,
                                 const std::string &value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  ClientContext context;
  Status status = stub_->put(&context, request, &reply);
}

std::vector<std::string> ClientForKeyValueStore::get(const std::string &key) {
  std::vector<std::string> replies;
  chirp::GetRequest request;
  request.set_key(key);

  ClientContext context;
  std::shared_ptr<grpc::ClientReaderWriter<GetRequest, GetReply>> stream(
      stub_->get(&context));

  stream->Write(request);
  stream->WritesDone();

  chirp::GetReply reply;
  while (stream->Read(&reply)) {
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
/*
    Implementing Functionalities for ServerForCommandLineClient
*/
ServerForCommandLineClient::ServerForCommandLineClient() {
  ClientForKeyValueStore client_key(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));
  /* This is to preset the number of chirps for the chirp ID. "chirps_" */
  std::vector<std::string> from_get = client_key.get("chirps_");
  if (from_get.size() == 0) {
    client_key.put("chirps_", std::to_string(0));
  }
}

Status ServerForCommandLineClient::registeruser(ServerContext *context,
                                                const RegisterRequest *request,
                                                RegisterReply *response) {
  ClientForKeyValueStore client_key(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));
  /*
    Tested: Able to add new users
  */
  auto from_get_function = client_key.get(request->username());
  if (from_get_function.size() != 0) {
    return Status::CANCELLED;
  }
  std::string string_to_user_proto;
  {
    chirp::User user_proto;
    user_proto.set_username(request->username());
    user_proto.SerializeToString(&string_to_user_proto);
  }
  client_key.put(request->username(), string_to_user_proto);
  return Status::OK;
}

Status ServerForCommandLineClient::follow(ServerContext *context,
                                          const FollowRequest *request,
                                          FollowReply *response) {
  ClientForKeyValueStore client_key(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));
  /*
    Tested: Able to add followers to user with an empty followers list and if
    there exist a followers list
  */
  if (request->username() == request->to_follow()) {
    return Status::CANCELLED;
  }

  std::string string_to_user_proto;
  {
    chirp::User user;
    std::vector<std::string> from_get_function =
        client_key.get(request->username());
    if (from_get_function.size() == 0) {
      return Status::CANCELLED;
    }

    std::vector<std::string> follow_exist =
        client_key.get(request->to_follow());
    if (follow_exist.size() == 0) {
      return Status::CANCELLED;
    }

    std::string string_username = from_get_function[0];
    user.ParseFromString(string_username);
    chirp::Followers *f = user.mutable_followers();
    for (int i = 0; i < f->username_size(); i++) {
      if (f->username(i) == request->to_follow()) {
        return Status::CANCELLED;
      }
    }
    f->add_username(request->to_follow());
    user.SerializeToString(&string_to_user_proto);
  }

  client_key.put(request->username(), string_to_user_proto);
  return Status::OK;
}

Status ServerForCommandLineClient::chirp(ServerContext *context,
                                         const ChirpRequest *request,
                                         ChirpReply *response) {
  ClientForKeyValueStore client_key(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));
  /*
    Tested: Able to add chirps! "chip<id>" is key and message Proto Chirp is
    value
  */
  /* Get TimeStamp */
  std::time_t seconds;
  int64_t microseconds_since_epoch;
  SetTimeStamp(seconds, microseconds_since_epoch);

  if (!CheckIfReplyIDExist(client_key, request)) {
    return Status::CANCELLED;
  }
  /*Get User*/
  chirp::User user;
  std::vector<std::string> from_get_function =
      client_key.get(request->username());
  if (from_get_function.size() == 0) {
    return Status::CANCELLED;
  }
  /* Get next chirp id*/
  std::string next_chirp_ID = GetNextChirpID(client_key);
  client_key.put("chirps_", next_chirp_ID);

  std::string getValue = from_get_function[0];
  user.ParseFromString(getValue);

  chirp::Chirp *message;
  /*Add new Chirp to chirp::User*/
  std::string create_chirp_to_proto;
  {
    message = user.add_chirps();
    message->set_username(request->username());
    message->set_text(request->text());
    message->set_id(next_chirp_ID);
    chirp::Timestamp *timestamp = message->mutable_timestamp();
    timestamp->set_seconds(static_cast<int64_t>(seconds));
    timestamp->set_useconds(microseconds_since_epoch);
    message->set_parent_id(request->parent_id());
    message->SerializeToString(&create_chirp_to_proto);

    SetChirpReply(message, response);
  }
  /* Save new chirp & User with new chirp back to database */
  std::string string_user_to_proto;
  user.SerializeToString(&string_user_to_proto);
  client_key.put(request->username(),
                 string_user_to_proto); /* Add Chirp to chirp::User */
  client_key.put("chirp" + next_chirp_ID,
                 create_chirp_to_proto); /* Add chirp::Chirp to database */

  /* If its a reply to an existing chirp */
  if (request->parent_id() != "") {
    std::string reply_chirp_to_proto;
    {
      std::vector<std::string> from_get_function =
          client_key.get("reply" + request->parent_id());

      chirp::ChirpReplies replies;
      if (from_get_function.size() == 0) {
        replies.set_parent_id(request->parent_id());
      } else { /* If found, parse from existing ChirpReplies */
        replies.ParseFromString(from_get_function[0]);
      }
      chirp::Chirp *reply = replies.add_chirps();
      CopyChirp(reply, *message);
      replies.SerializeToString(&reply_chirp_to_proto);
    }
    client_key.put("reply" + request->parent_id(),
                   reply_chirp_to_proto); /* Add reply<ID> to backend */
  }
  return Status::OK;
}

Status ServerForCommandLineClient::read(ServerContext *context,
                                        const ReadRequest *request,
                                        ReadReply *response) {
  /*
    Tested: Able to get all chirps and it's replies and send it back via
    ReadReply
  */
  ClientForKeyValueStore client_key(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  std::vector<std::string> from_get_function =
      client_key.get("chirp" + request->chirp_id());
  std::vector<chirp::Chirp> chirpToSend; // For sorting by timestamp
  if (from_get_function.size() == 0) {
    return Status::CANCELLED;
  }
  std::string main_chirp_msg = from_get_function[0];

  chirp::Chirp *main_chirp = response->add_chirps();
  main_chirp->ParseFromString(main_chirp_msg);
  chirpToSend.push_back(*main_chirp);

  std::queue<chirp::Chirp> myqueue;
  myqueue.push(*main_chirp);

  /*
    Running DFS to get threads related to chirp<id>
  */
  while (!myqueue.empty()) {
    chirp::Chirp current_chirp = myqueue.front();
    myqueue.pop();
    /*
      Find other chirps that are in reply to this current_chirp
    */
    from_get_function = client_key.get("reply" + current_chirp.id());
    if (from_get_function.size() == 0) {
      continue;
    }

    chirp::ChirpReplies chirp_replies =
        ConvertToChirpReplies(from_get_function[0]);
    for (int i = 0; i < chirp_replies.chirps_size(); i++) {
      myqueue.push(chirp_replies.chirps(i));
      chirpToSend.push_back(chirp_replies.chirps(i));
    }
  }
  /*
    Sort by timestamp
  */
  std::sort(chirpToSend.begin(), chirpToSend.end(),
            [](chirp::Chirp &a, chirp::Chirp &b) {
              return a.timestamp().useconds() < b.timestamp().useconds();
            });
  for (const auto &i : chirpToSend) {
    if (i.id() == main_chirp->id()) {
      continue;
    }
    chirp::Chirp *c = response->add_chirps();
    CopyChirp(c, i);
  }
  return Status::OK;
}

Status ServerForCommandLineClient::monitor(
    ServerContext *context, const MonitorRequest *request,
    ::grpc::ServerWriter<::chirp::MonitorReply> *writer) {
  /* Get Current TimeStamp */
  std::time_t seconds;
  int64_t microseconds_since_epoch;
  SetTimeStamp(seconds, microseconds_since_epoch);

  ClientForKeyValueStore client_key(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  auto from_get_function = client_key.get(request->username());
  if (from_get_function.size() == 0) {
    return Status::CANCELLED;
  }

  std::set<std::string> chirpsent;
  chirp::User user; // Main User
  user.ParseFromString(from_get_function[0]);

  chirp::MonitorReply reply;
  chirp::Followers followers = user.followers();
  /*
    Continuously look through all user's followers, and their chirps. Keep all
    sent chirps in a set called chirpsent to disallow sending duplicates. Keep
    looking for new chirps with this while loop.
  */
  while (true) {
    if (context->IsCancelled()) {
      break;
    }
    for (int i = 0; i < followers.username_size(); i++) {
      auto all_followers = client_key.get(followers.username(i));

      if (all_followers.size() > 0) {
        chirp::User curr_follower;
        std::string temp = all_followers[0];
        curr_follower.ParseFromString(temp);
        /* Loop through this User's follower's chirps */
        for (int j = 0; j < curr_follower.chirps_size(); j++) {
          auto it = chirpsent.find(curr_follower.chirps(j).id());
          /* Find this User's follower's chirp's id in the set. If it's not in
            the set, then its a new chirp */
          if (it == chirpsent.end()) {
            /* If this chirp is chirped after creating montior */
            if (curr_follower.chirps(j).timestamp().useconds() >
                microseconds_since_epoch) {
              chirpsent.insert(curr_follower.chirps(j).id());

              chirp::Chirp *c = reply.mutable_chirp();
              CopyChirp(c, curr_follower.chirps(j));
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
/*
  Below: Private Functions Implementations
*/
void ServerForCommandLineClient::CopyChirp(chirp::Chirp *chirp,
                                           const chirp::Chirp &chirp_to_copy) {
  chirp->set_username(chirp_to_copy.username());
  chirp->set_text(chirp_to_copy.text());
  chirp->set_id(chirp_to_copy.id());
  chirp->set_parent_id(chirp_to_copy.parent_id());
  chirp::Timestamp *time = chirp->mutable_timestamp();
  time->set_seconds(chirp_to_copy.timestamp().seconds());
  time->set_useconds(chirp_to_copy.timestamp().useconds());
}

chirp::Chirp ServerForCommandLineClient::ConvertToChirp(std::string byte) {
  chirp::Chirp chirp;
  chirp.ParseFromString(byte);
  return chirp;
}

chirp::ChirpReplies
ServerForCommandLineClient::ConvertToChirpReplies(std::string byte) {
  chirp::ChirpReplies replies;
  replies.ParseFromString(byte);
  return replies;
}

chirp::User ServerForCommandLineClient::StringToUser(std::string byte) {
  chirp::User user;
  user.ParseFromString(byte);

  return user;
}

void ServerForCommandLineClient::SetChirpReply(chirp::Chirp *chirp,
                                               chirp::ChirpReply *response) {
  chirp::Chirp *new_chirp = response->mutable_chirp();
  new_chirp->set_username(chirp->username());
  new_chirp->set_text(chirp->text());
  new_chirp->set_id(chirp->id());
  new_chirp->set_parent_id(chirp->parent_id());
  chirp::Timestamp *timestamp = new_chirp->mutable_timestamp();
  timestamp->set_seconds(chirp->timestamp().seconds());
  timestamp->set_useconds(chirp->timestamp().useconds());
}

std::string
ServerForCommandLineClient::GetNextChirpID(ClientForKeyValueStore &client_key) {
  auto from_get_function = client_key.get("chirps_");
  std::string next_chirp_ID;
  if (from_get_function.size() == 0) {
    next_chirp_ID = std::to_string(0);
  } else {
    int curr_chirp_id = std::stoi(from_get_function[0]);
    curr_chirp_id++;
    next_chirp_ID = std::to_string(curr_chirp_id);
  }
  return next_chirp_ID;
}

void ServerForCommandLineClient::SetTimeStamp(
    std::time_t &seconds, int64_t &microseconds_since_epoch) {
  seconds = std::time(nullptr);
  microseconds_since_epoch =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();
}

bool ServerForCommandLineClient::CheckIfReplyIDExist(
    ClientForKeyValueStore &client_key, const ChirpRequest *request) {
  if (request->parent_id() != "") {
    auto from_get_function = client_key.get("chirp" + request->parent_id());
    if (from_get_function.size() == 0) {
      return false;
    }
  }
  return true;
}
