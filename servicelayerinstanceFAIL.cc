#include "servicelayerinstance.h"
  // std::string ChirpToString(ChirpObject chirp); /* Converts Chirp to String*/
  // ChirpObject StringToChirp(std::string chirp); /* Converts String to Chirp */
  // ReplyObject StringToReplyObject(std::string reply); /* Converts String to ReplyObject*/
  // std::string ReplyObjToString(ReplyObject reply); /* Converts ReplyObject to String*/
  // std::vector <ChirpObject> VectorStringToChirps(std::string chirps, int length); /* Converts list of chirps to vector of ChirpObjects */
  // UserObject StringToUserObject(std::string user); /* Convert String to UserObject*/
  // std::string UserObjectToString(UserObject userobj); /* Convert UserObject to String */
ServiceLayerInstance::ServiceLayerInstance(KeyValueStoreInstance *kv_instance) {
  kvstore = kv_instance;
  if (kvstore->Get("chirpcount") == "") {
    kvstore->Put("chirpcount", std::to_string(0));
  }
}

bool ServiceLayerInstance::RegisterUser(const std::string &username) {
  if (username != "") {
    kvstore->Put(username, "registered");
    return true;
  }
  return false;
}

std::string ServiceLayerInstance::ChirpToString(ChirpObject chirp) {
  
  std::string string_chirp_obj;
  string_chirp_obj = chirp.chirpID_ + "/" + chirp.parentID_ +
  "/"+ chirp.text_ +"/" + chirp.username_;
  return string_chirp_obj;
}

std::string ServiceLayerInstance::ReplyObjToString(ReplyObject reply) {
  
  std::string string_reply_obj;
  string_reply_obj = reply.username_ + "/" + reply.parentID_ + "/" + std::to_string(reply.replies_.size());
  for (int i = 0; i < reply.replies_.size(); i++) {
    string_reply_obj += "/" + ChirpToString(reply.replies_[i]);
  }
  std::cout << string_reply_obj <<std::endl;
  return string_reply_obj;
}

ChirpObject ServiceLayerInstance::StringToChirp(std::string chirp) {
  
  ChirpObject chirp_obj;
  int i = 0;
  int curr_index = 0;
  std::string chirp_id, parent_id = "", text, user_name;
  while (curr_index != (chirp.length())) {

    if (chirp.at(curr_index) == '/') {
      i++;
    } else if (i == 0) {
      chirp_id += chirp.at(curr_index);
    } else if (i == 1) {
      parent_id += chirp.at(curr_index);
    } else if (i == 2) {
      text += chirp.at(curr_index);
    } else if (i == 3) {
      user_name += chirp.at(curr_index);
    }
    curr_index++;
  }
  chirp_obj.chirpID_ = chirp_id;
  chirp_obj.parentID_ = parent_id;
  chirp_obj.text_ = text;
  chirp_obj.username_ = user_name;

  return chirp_obj;
}

std::vector <ChirpObject> ServiceLayerInstance::VectorStringToChirps(std::string chirps, int length) {

  int beginning = 0;
  int curr_index = 0;
  int slashcounter = 0;
  std::vector <ChirpObject> allChirps;
  while (curr_index != chirps.length() && length != 0) {

    if (chirps.at(curr_index) == '/') {
      slashcounter++;

    } else if (slashcounter == 4 && length > 1) {
      

      ChirpObject made_chirp_obj = StringToChirp(chirps.substr(beginning,curr_index - beginning - 1));
      allChirps.push_back(made_chirp_obj);
      beginning = curr_index;
      slashcounter = 0;
      length--;
    } else if (slashcounter == 3 && length == 1) { 

      ChirpObject made_chirp_obj = StringToChirp(chirps.substr(beginning, chirps.size() - beginning));
      allChirps.push_back(made_chirp_obj);
      
      slashcounter = 0;
      length--;
    }
    curr_index++;
  }
  return allChirps;
}

ReplyObject ServiceLayerInstance::StringToReplyObject(std::string reply) {
  
  ReplyObject reply_obj;
  int i = 0;
  int curr_index = 0;
  std::string username, parentID;
  std::string num_chirp_obj;
  int num_chirps = 0;
  std::vector <ChirpObject> replies;
 
  while (curr_index != reply.length()) {
    if (reply.at(curr_index) == '/') {
      i++;
    } else if (i == 0) {
      username += reply.at(curr_index);
    } else if (i == 1) {
      parentID += reply.at(curr_index);
    } else if (i == 2) { 
      num_chirp_obj += reply.at(curr_index);
    } else if (i == 3) {
      num_chirps = std::stoi(num_chirp_obj);
      replies = VectorStringToChirps(reply.substr(curr_index, reply.length() - curr_index + 1), num_chirps);
      break;
    }
    curr_index++;
  }
  reply_obj.username_ = username;
  reply_obj.parentID_ = parentID;
  reply_obj.replies_ = replies;
  return reply_obj;
}

UserObject ServiceLayerInstance::StringToUserObject(std::string user) {
  UserObject user_obj;
  int i = 0;
  int curr_index = 0;
  std::string username;
  std::vector <std::string> followers;
  std::vector <ChirpObject> chirps;
  std::string num_followers, num_chirps;
  while (curr_index != user.length()) {

    if (user.at(curr_index) == '/') {
      i++;
    } else if (i == 0) {
      username += user.at(curr_index);
    } else if (i == 1) {
      num_followers += user.at(curr_index);
    } else if (i == 2) {
      int followers_size = std::stoi(num_followers);
      int j = 0;
      std::string followers_name = "";
      int count_index = curr_index;
      while (j < followers_size) {

        if (user.at(count_index) == '/') {

          followers.push_back(followers_name);
          followers_name = "";
          count_index++;
          j++;
        } else {
          followers_name += user.at(count_index);
          count_index++;
        }
      }
      curr_index = count_index - 2;

    } else if (i == 3) {
      num_chirps += user.at(curr_index);
    } else if (i == 4) {
      int int_num_chirps = std::stoi(num_chirps);

      chirps = VectorStringToChirps(user.substr(curr_index, user.length() - curr_index), int_num_chirps);
      break;
    }
    curr_index++;
  }

  user_obj.username_ = username;
  user_obj.followers_ = followers;
  user_obj.chirps_ =  chirps;
  return user_obj;
}

std::string ServiceLayerInstance::UserObjectToString(UserObject userobj) {
  std::string string_user = userobj.username_ + "/" + std::to_string(userobj.followers_.size());
  for (int i = 0; i < userobj.followers_.size(); i++) {
    string_user += "/" + userobj.followers_[i];
  }
  string_user += "/" + std::to_string(userobj.chirps_.size());
  for (int i = 0; i < userobj.chirps_.size(); i++) {
    string_user += "/" + ChirpToString(userobj.chirps_[i]);
  }
  return string_user;
}

void ServiceLayerInstance::Chirp(const std::string &username, const std::string &text, const std::string &parentID) {
  ChirpObject chirp_obj;
  chirp_obj.username_ = username;
  chirp_obj.text_ = text;
  chirp_obj.parentID_ = parentID;
  std::string chirpcount = kvstore->Get("chirpcount");
  
  if (chirpcount != "") {
    chirp_obj.chirpID_ = std::to_string(std::stoi(chirpcount) + 1);
    kvstore->Put("chirpcount", chirp_obj.chirpID_);
  } else {
    chirp_obj.chirpID_ = std::to_string(1);
    kvstore->Put("chirpcount", std::to_string(1));
  }
  kvstore->Put("chirp"+ chirp_obj.chirpID_, ChirpToString(chirp_obj));

  if (parentID != "") {
    std::string ReplyChirp = kvstore->Get("reply"+parentID);
    ReplyObject reply;
    if (ReplyChirp != "") {
      reply = StringToReplyObject(ReplyChirp);
      reply.replies_.push_back(chirp_obj);
    } else {
      reply.username_ = username;
      reply.parentID_ = parentID;
      reply.replies_.push_back(chirp_obj);
    }
    kvstore->Put("reply"+parentID, ReplyObjToString(reply));
  } 
}

bool ServiceLayerInstance::Follow(const std::string &username, const std::string &toFollow) {
  
}

std::vector <std::string> ServiceLayerInstance::Read(const std::string &chirpcount) {

}
std::vector <std::string> ServiceLayerInstance::Monitor(const std::string &username) {

}