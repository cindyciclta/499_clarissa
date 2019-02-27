#include "servicelayervirtual.h"
#include "keyvaluestoreinstance.h"

class ServiceLayerInstance : public ServiceLayerVirtual {
 public:
  ServiceLayerInstance();
  bool registeruser(const std::string &username);
  void chirp(const std::string &username, const std::string &text, const std::string &parentID);
  bool follow(const std::string &username, const std::string &toFollow);
  std::vector <std::string> read(const std::string &chirpID);
  std::vector <std::string> monitor(const std::string &username);
 private:
  KeyValueStoreInstance kvstore; //Key Value Store for Testing purpose
  std::string chirpToString(ChirpObject c);
  ChirpObject stringToChirp(std::string chirp);
  ReplyObject stringToReplyObject(std::string reply);
};

/*
  UserObject is an object to store all users who registers. For testing purposes only.
  Actual UserObject is using grpc protos. However, this resembles the functionality of the proto object User.
*/
struct UserObject {
  std::string username_;
  std::vector <std::string> followers_;
  std::vector <Chirps> chirps_;
};

struct ChirpObject {
  int chirpID_ = 0;
  int parentID_ = 0;
  std::string text_;
  time_t timestamp_;
  std::string username_;
};

struct ReplyObject {
  std::string username_;
  int parentID_ = 0;
  std::vector <ChirpObject> replies_;
}