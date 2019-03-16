#ifndef SERVICELAYERINSTANCE_H
#define SERVICELAYERINSTANCE_H
#include "chirp.grpc.pb.h"
#include "servicelayervirtual.h"
#include "keyvaluestoreinstance.h"
/*
  UserObject is an object to store all users who registers. For testing purposes only.
  Actual UserObject is using grpc protos. However, this resembles the functionality of the proto object User.
*/
struct ChirpObject {
  std::string chirpID_;
  std::string parentID_;
  std::string text_;
  std::string username_;
};

struct ReplyObject {
  std::string username_;
  std::string parentID_;
  std::vector <ChirpObject> replies_;
};

struct UserObject {
  std::string username_;
  std::vector <std::string> followers_;
  std::vector <ChirpObject> chirps_;
};
/*
  This Class is to test the service layer functionalities. It does not
  go over GRPC. It uses KeyValueStoreInstance which is a key value store
  that does need GRPC to get the object.
*/
class ServiceLayerInstance : public ServiceLayerVirtual {
 public:
  ServiceLayerInstance(KeyValueStoreInstance *kv_instance);
  ~ServiceLayerInstance() {};
  bool RegisterUser(const std::string &username);
  bool Chirp(const std::string &username, const std::string &text, const std::string &parentid);
  bool Follow(const std::string &username, const std::string &tofollow);
  std::vector <chirp::Chirp> Read(const std::string &chirpid);
  std::vector <chirp::Chirp> Monitor(const std::string &username);
  /*
    The functions below are helper functions to serialize and parse objects
    this is made public so it can be tested in servicelayertest.cc
  */
  /*
    Converts serialized string to chirp::Chirp
  */
  chirp::Chirp ConvertToChirp(std::string byte);
  /*
    Converts serialized string to chirp::ChirpReplies (which is a list of replies to the same chirp<ID>)
  */
  chirp::ChirpReplies ConvertToChirpReplies(std::string byte);
  /*
    Copy chirps
  */
  void CopyChirp(chirp::Chirp* c, const chirp::Chirp &r);
  /*
    Converts serialize string to chirp::User.
  */
  chirp::User StringToUser(std::string byte);
  /*
    Copy chirp to ChirpReply's chirp.
  */
  void SetChirpReply(chirp::Chirp* chirp, chirp::ChirpReply* response);
 private:
  KeyValueStoreInstance *kvstore; /* Key Value Store for Testing purpose */
  /*
    Mutex to lock chirps_ to ensure there is no duplicated chirp id.
  */
  std::mutex mymutex_;

};
#endif
