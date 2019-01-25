#ifndef USER_H_
#define USER_H_

#include <string>
#include <vector>

/**
	User class is to store every user's information when a user registers! This will be used in the backend.cc
*/
class User {
	public:
		explicit User(const std::string name);
		void Chirp(const std::string chirp); //Save all the chirps from this user
		void SetID(int id); //Chirp unique ID
		int GetId() const; //Gets Chirp unique ID
	private:
		std::vector<std::string> chirps_; //To store all the chirps this user sent
		int chirpID_; //Chirp unqiue ID
		std::string name_; // User username
		std::vector<std::string> followers_; //All this user's followers
};

#endif //USER_H_