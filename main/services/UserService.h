#pragma once
#include <string>
#include <vector>
#include <optional>
#include "repositories/UserRepository.h"
#include "repositories/BookRepository.h"
#include "models/Date.h"
#include "models/Message.h"

class UserService
{
private:

	UserRepository& userRepo;
	BookRepository& bookRepo;

public:

	UserService(UserRepository& userRepo, BookRepository& bookRepo);

	std::pair<std::vector<const User*>, std::vector<const Book*>> search(const std::string& query) const;

	void followUser(const std::string& fromUsername, const std::string& targetUsername);

	std::vector<const User*> getFriends(const std::string& requesterUsername, std::optional<std::string> targetUsername = std::nullopt) const;

	const User* getProfile(const std::string& requesterUsername, std::optional<std::string> targetUsername = std::nullopt) const;

	void addBirthday(const std::string& username, const Date& birthday);

	void clearBirthday(const std::string& username);

	std::vector<const Message*> getInbox(const std::string& username, std::optional<MessageType> filter = std::nullopt) const;

	void readMessage(const std::string& username, int messageIndex);

	void deleteMessage(const std::string& username, int messageIndex);

	void acceptJobOffer(const std::string& username, int messageIndex);

	void leaveJob(const std::string& authorUsername, const std::string& publisherUsername);

	std::vector<const User*> getFollowers(const std::string& authorUsername) const;

	void sendOffer(const std::string& publisherUsername, const std::string& authorUsername);
};
