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
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  ClientFunctionalities client(grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials()));

  if (FLAGS_registeruser != "") {
    client.registeruser(FLAGS_registeruser);
  } else if (FLAGS_user != "") {
    if (FLAGS_chirp != "" && FLAGS_reply == "") {
      client.chirp(FLAGS_user, FLAGS_chirp, "");
    } else if (FLAGS_chirp != "" && FLAGS_reply != "") {
      client.chirp(FLAGS_user, FLAGS_chirp, FLAGS_reply);
    }

    if (FLAGS_follow != "") {
      client.follow(FLAGS_user, FLAGS_follow);
    }

    if (FLAGS_read != "") {
      client.read(FLAGS_read);
    }
    if (FLAGS_monitor) {
      client.monitor(FLAGS_user);
    }
  }
  //TODO output errors 
  return  0;
}

