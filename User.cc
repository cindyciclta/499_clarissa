#include "user.h"

/**
  Implmentation of user.h
*/
User::User(const std::string &name, int chirpID) {
  name_ = name;
  chirpID_ = chirpID;
}
void User::Chirp(const std::string &chirp) {
  chirps_.push_back(chirp);
}
void User::SetID(int id) {
  chirpID_ = id;
}
int User::GetId() const {
  return chirpID_;
}