#include <thread>
#include <unistd.h>

#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>

#include "clientfunctionalities.h"

/**
    Main function where the command line tool will be implemented. 
    TODO: Create a full implmentation of the command line tool
**/
DEFINE_string(user,"", "username of user using chrip");
DEFINE_string(registeruser, "", "register a new user with a username");
DEFINE_string(chirp, "", "text of a new chirp from user");
DEFINE_string(reply, "", "replies a chirp with a new chirp");
DEFINE_string(follow, "", "follows another user");
DEFINE_string(read, "", "reads the chirp thread starting at the given id");
DEFINE_bool(monitor,false, "streams new chirps from those currently followed");

static bool ValidateUser(const char* flagname, const std::string &value) {
  if (FLAGS_registeruser != "" || value != "") {
    return true;
  }
  return false;
}

static bool ValidateChirp(const char* flagname, const std::string &value) {
  if (value != "") {
    return true;
  }
  return false;
}
static bool ValidateReply(const char* flagname, const std::string &value) {
  if (value != "") {
    return ValidateChirp("chirp",FLAGS_chirp);
  }
  return true;
}



DEFINE_validator(user, &ValidateUser);
DEFINE_validator(reply, &ValidateReply);

int main(int argc, char** argv) {
  // gflags::ParseCommandLineFlags(&argc, &argv, true);

  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  
  client.registeruser("user1");
  client.registeruser("user2");
  client.registeruser("user3");
  client.follow("user1", "user2");
  client.follow("user2", "user1");

  client.chirp("user1", "testing chirp", "none");
  client.chirp("user1", "testing chirp222222", "1");
  client.chirp("user2", "replying to 1", "1");
  client.monitor("user2");
  auto chirpthread = client.read("1");

  for(const auto& i : chirpthread) {
    std::cout << i.first << ": "<< i.second << std::endl;
  }
  return  0;
}

