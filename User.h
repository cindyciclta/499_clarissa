#include <string>
#include <vector>
class User{
public:
	User(std::string name);
	void Chirp(std::string chirp);
	void SetID(int id);
	int GetId();
private:
	std::vector<std::string> chirps;
	int chirpID;
	std::string name;
	std::vector<std::string> followers;
};