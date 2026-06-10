#pragma once
#include <string>
#include <vector>
#include <optional>
#include "repositories/UserRepository.h"
#include "repositories/BookRepository.h"
#include "models/Date.h"
#include "models/Message.h"
#include "users/Reader.h"
#include "users/Author.h"
#include "users/Publisher.h"

class UserService
{
private:

	UserRepository& userRepo;
	BookRepository& bookRepo;

public:

	UserService(UserRepository& userRepo, BookRepository& bookRepo);

	std::pair<std::vector<const User*>, std::vector<const Book*>> search(const std::string& query) const;

	void followUser(User* currUser, const std::string& targetUsername);

	std::vector<const User*> getFriends(User* currUser, std::optional<std::string> targetUsername = std::nullopt) const;

	const User* getProfile(User* currUser, std::optional<std::string> targetUsername = std::nullopt) const;

	void addBirthday(Reader* currReader, const Date& birthday);

	void clearBirthday(Reader* currReader);

	std::vector<const Message*> getInbox(User* currUser, std::optional<MessageType> filter = std::nullopt) const;

	void readMessage(User* currUser, int messageIndex);

	void deleteMessage(User* currUser, int messageIndex);

	void acceptJobOffer(Author* currAuthor, int messageIndex);

	void leaveJob(Author* currAuthor, const std::string& publisherUsername);

	std::vector<const User*> getFollowers(Author* currAuthor) const;

	void sendOffer(Publisher* currPublisher, const std::string& authorUsername);
};
