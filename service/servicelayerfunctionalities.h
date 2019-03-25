#ifndef SERVICE_LAYER_FUNCTIONALITIES_H_
#define SERVICE_LAYER_FUNCTIONALITIES_H_
#include <iostream>
#include <memory>
#include <string>
#include <queue>
#include <thread>
#include <set>
#include <ctime>
#include <ratio>
#include <chrono>

#include <grpcpp/grpcpp.h>
#include <grpcpp/client_context.h>
#include "../chirp.grpc.pb.h"

#include "clientforkeyvaluestore.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using chirp::Chirp;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::PutReply;
using chirp::PutRequest;
using chirp::GetRequest;
using chirp::DeleteRequest;
using chirp::DeleteReply;
using chirp::KeyValueStore;

using chirp::RegisterRequest;
using chirp::RegisterReply;
using chirp::ChirpRequest;
using chirp::ChirpReply;
using chirp::FollowRequest;
using chirp::FollowReply;
using chirp::ReadRequest;
using chirp::ReadReply;
using chirp::MonitorRequest;
using chirp::MonitorReply;
using chirp::ServiceLayer;

using namespace std::chrono;
using time_stamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

/*
    ServerForCommandLineClient class is for the functionalties that would be called from the ClientCommandLine client.
    It will handle requests from the ClientCommandLine client and sends a response. AKA the Server to
    the command line client (clientcommandline.cc).
*/
class ServerForCommandLineClient final : public ServiceLayer::Service {
 public:
  /*
    Constructor that will call get("chirp_") from ClientForkeyValueStore class. The purpose is to initalize
    chirps_ (which counts the total number of chirps for chirp<ID>). If "chirps_" doesn't exist then there
    is no chirps in the database, so chirps_ will start at 0;
  */
  ServerForCommandLineClient();
  /*
    registeruser() takes in a ServerContext from the client that calls this function, as well as
    RegisterRequest, and Register Reply. The command line client can call this function to register a new user via Register Request.
    This function will create a new chirp::User, input the username and put it in the database
    "{username}" is the key, chirp::User is the value. RegisterReply doesn't reply anything because
    success/failure is signaled via GRPC status.
  */
  Status registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response);
  /*
    chirp() takes in a ServerContext from the client that calls this function, as well as,
    ChirpRequest, and ChirpReply. chirp() will take the set values in ChirpRequest and
    create a chirp::Chirp. This function will put the new chirp into the database
    "chirp<ID>" is the key, chirp::Chirp is the value. Returns the created chirp::Chirp in ChirpReply.
  */
  Status chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response);
  /*
    follow() takes in a ServerContext the client that calls this function, as well as,
    FollowRequest, and FollowReply. follow() will take a request via FollowRequest, finds the
    username in the database, add a new follower to the username if exists. Updates the username's
    follower list. FollowReply doesn't reply anything success/failure is signaled via GRPC status.
  */
  Status follow(ServerContext* context, const FollowRequest* request, FollowReply* response);
  /*
    read() takes in a ServerContext the client that calls this function, as well as,
    ReadRequest and ReadReply. ReadRequest provides a chirp_id. read() will call the get()
    function in ClientForKeyvalueStore and then retrieves the chirp with that id, as well
    as any chirps that replied to that chirp_id, and onwards. read() does a DFS and returns
    a list of chirps in ReadReply.
  */
  Status read(ServerContext* context, const ReadRequest* request, ReadReply* response);
  /*
    monitor takes in a ServerContext the client that calls this function, as well as,
    MonitorRequest, and ::grpc::ServerWriter<::chirp::MonitorReply>. MonitorRequest will
    allow monitor() to find the specific username in the database. If found, monitor will
    endlessly sends all the chirps from anyone the user follows, and sends new chirps in real-time.
  */
  Status monitor(ServerContext* context, const MonitorRequest* request, ::grpc::ServerWriter<::chirp::MonitorReply>* writer);
 private:
  /*
    Mutex to lock chirps_ to ensure there is no duplicated chirp id.
  */
  std::mutex mymutex_;
  /*
    Converts serialized string (byte) to chirp::Chirp
  */
  chirp::Chirp ConvertToChirp(std::string byte);
  /*
    Converts serialized string (byte) to chirp::ChirpReplies (which is a list of replies to the same chirp<ID>)
  */
  chirp::ChirpReplies ConvertToChirpReplies(std::string byte);
  /*
    Copy chirps, "chirp" is an empty chirp, and "chirp_to_copy" is copying into "chirp"
  */
  void CopyChirp(chirp::Chirp *chirp, const chirp::Chirp &chirp_to_copy);
  /*
    Converts serialize string to chirp::User.
  */
  chirp::User StringToUser(std::string byte);
  /*
    Copy chirp to ChirpReply's chirp.
  */
  void SetChirpReply(chirp::Chirp* chirp, chirp::ChirpReply* response);
  /*
    Get the next chirp ID from backend
  */
  std::string GetNextChirpID(ClientForKeyValueStore &client_key);
  /*
    Set TimeStamp: takes in time_t and int64_t and sets the time accordingly
  */
  void SetTimeStamp(std::time_t &seconds, int64_t &microseconds_since_epoch);
  /*
    Check if the chirp that the user is replying to exist.
    Takes in client_key to call Get() and ChirpRequest to get the id we are testing.
  */
  bool CheckIfReplyIDExist(
      ClientForKeyValueStore &client_key, const ChirpRequest *request);
  /*
    Helper function for read() to sort chirps by timestamp.
    Takes in the vector that needs to be sorted and the ReadReply response
    is taken so the list of sorted chirps which will be sent to the client.
  */
  void SortReadChirpByTimestamp(
      std::vector<Chirp> &chirp_to_send, ReadReply *response,
      chirp::Chirp *main_chirp);
};
#endif /* SERVICE_LAYER_FUNCTIONALITIES_H_ */
