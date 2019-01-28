#ifndef USER_H_
#define USER_H_

#include <string>
#include <vector>

/**
	User class is to store every user's information when a user registers! This will be used in the backend.cc
*/
class User {
 public:
  //Constructor that takes in a username
  explicit User(const std::string &name, int chirpID);
  //Save all the chirps from this user 
  void Chirp(const std::string &chirp); 
  //Chirp unique ID
  void SetID(int id); 
  //Gets Chirp unique ID
  int GetId() const; 
  
 private:
  //To store all the chirps this user sent
  std::vector<std::string> chirps_; 
  //Chirp unqiue ID
  int chirpID_; 
  // User username
  std::string name_; 
  //All this user's followers
  std::vector<std::string> followers_; 
};

#endif //USER_H_