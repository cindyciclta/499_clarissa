#include "servicelayerinstance.h"

ServiceLayerInstance::ServiceLayerInstance() {
  if (kvstore.get("chirpcount") != null) {
    kvstore.put("chirpcount", 0);
  }

}
bool ServiceLayerInstance::registeruser(const std::string &username) {
  if (username != "") {
    return kvstore.put(username, "registered");
  }
  return false;
    
}
std::string ServiceLayerInstance::chirpToString(ChirpObject c) {

}
ChirpObject ServiceLayerInstance::stringToChirp(std::string chirp) {

}
ReplyObject ServiceLayerInstance::stringToReplyObject(std::string reply) {

}
void ServiceLayerInstance::chirp(const std::string &username, const std::string &text, const std::string &parentID) {
  ChirpObject c;
  c.username_ = username;
  c.text_ = text;
  c.parentID_ = parentID;
  std::string chirpcount = kvstore.get("chirpcount");
  if (chirpcount != NULL) {
    c.chirpID_ = stoi(chirpcount) + 1;
    kvstore.put("chirpcount", std::to_string(c.chirpID_));
  } else {
    c.chirpID_ = 1;
    kvstore.put("chirpcount", std::to_string(1));
  }
  kvstore.put("chirp"+ std::to_string(c.chirpID_), chirpToString(c));

  if (parentID != "") {
    std::string ReplyChirp = kvstore.get("reply"+parentID);
    if (ReplyChirp != NULL) {
      ReplyObject = stringToReplyChirp(ReplyChirp);
      replies.push_back(c);
    } else {
      ReplyObject reply;
      reply.username_ = username;
      reply.parentID_ = parentID;
      reply.replies.push_back(c);
    }
  } 
}

bool ServiceLayerInstance::follow(const std::string &username, const std::string &toFollow) {
  
}
std::vector <std::string> ServiceLayerInstance::read(const std::string &chirpcount) {

}
std::vector <std::string> ServiceLayerInstance::monitor(const std::string &username) {

}