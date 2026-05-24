#include "repositories/UserRepository.h"
#include <fstream>
#include <stdexcept>
#include "users/Reader.h"
#include "users/Author.h"
#include "users/Publisher.h"

UserRepository::UserRepository(const std::string& filePath) : filePath(filePath)
{
	loadFromFile();
}

UserRepository::~UserRepository()
{
	saveToFile();
}

const User* UserRepository::findByUsername(const std::string& username) const
{
	const User* user = findWhere([&username](const User& u){
		return u.getUsername() == username;
	});

	if (!user){
		throw std::runtime_error("User " + username + " not found");
	}

	return user;
}

bool UserRepository::usernameExists(const std::string& username) const
{
	return findWhere([&username](const User& u){
		return u.getUsername() == username;
	}) != nullptr;
}

std::vector<const User*> UserRepository::getFollowersOf(const std::string& username) const
{
	return findAllWhere([&username](const User& u){
		return u.isFollowing(username);
	});
}

std::vector<const User*> UserRepository::getFriendsOf(const std::string& username) const
{
	const User* target = findByUsername(username);

	return findAllWhere([&username, target](const User& u){
		return u.getUsername() != username
			&& u.isFollowing(username)
			&& target->isFollowing(u.getUsername());
	});
}

User* UserRepository::getMutable(const std::string& username)
{
	User* user = findMutable([&username](const User& u){
		return u.getUsername() == username;
	});

	if (!user) {
		throw std::runtime_error("User " + username + " not found");
	}

	return user;
}

std::vector<User*> UserRepository::getAllMutable()
{
	return findAllMutable([](const User&) {
		return true; 
	});
}

void UserRepository::loadFromFile()
{
	std::ifstream in(filePath);
	if (!in.is_open()) {
		return;
	}

	size_t count;
	in >> count;
	in.ignore();

	for (size_t i = 0; i < count; i++){
		std::string type;
		std::getline(in, type);

		std::unique_ptr<User> user;
		if (type == "Reader") {
			user = std::make_unique<Reader>();
		}
		else if (type == "Author") {
			user = std::make_unique<Author>();
		}
		else if (type == "Publisher") {
			user = std::make_unique<Publisher>();
		}
		else {
			throw std::runtime_error("UserRepository: unknown user type in file!");
		}

		user->deserialize(in);
		data.push_back(std::move(user));
	}
}

void UserRepository::saveToFile() const
{
	std::ofstream out(filePath);
	if (!out.is_open()) {
		throw std::runtime_error("UserRepository: cannot open file for writing!");
	}

	out << data.size() << '\n';
	for (const auto& user : data){
		user->serialize(out);
	}
}
