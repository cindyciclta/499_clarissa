#include <thread>
#include <unistd.h>

#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>

#include "clientfunctionalities.h"

/**
    Main function where the command line tool will be implemented. 
    TODO: Create a full implmentation of the command line tool
**/
// DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
// DEFINE_string(languages, "english,french,german", "comma-separated list of languages to offer in the 'lang' menu");
DEFINE_string(user,"", "username of user using chrip");
DEFINE_string(registeruser, "Register a new user with Chirp.", "register a new user with a username");
DEFINE_string(chirp, "New Chirp.", "text of a new chirp from user");
DEFINE_string(reply, "Reply to a chirp.", "replies a chirp with a new chirp");
DEFINE_string(follow, "Follow another user.", "follows another user");
DEFINE_string(read, "Read a chirp thread.", "reads the chirp thread starting at the given id");
DEFINE_string(monitor, "Monitor all follower's chirps", "streams new chirps from those currently followed");
static bool ValidateUser(const char* flagname, const std::string &value) {
  if (value != "") {
    return true;
  }
  return false;
}
DEFINE_validator(user, &ValidateUser);
int main(int argc, char** argv) {
  // gflags::ParseCommandLineFlags(&argc, &argv, true);
  // if(FLAGS_user == "") {
  //   std::cout << "Error, must have --user {username}" <<std::endl;
  //   return -1;
  // }
  // std::cout << FLAGS_registeruser <<std::endl;
  // std::cout << FLAGS_user << std::endl;
  // std::cout << FLAGS_chirp << std::endl;
  // std::cout<< "FLAGS_big_menu : " << FLAGS_big_menu << "\n" << FLAGS_languages << std::endl;
  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));
  
  client.registeruser("user1");
  client.registeruser("user2");
  // client.registeruser("user3");
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

