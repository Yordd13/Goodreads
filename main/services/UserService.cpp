#include "services/UserService.h"
#include "users/Reader.h"
#include "users/Author.h"
#include "users/Publisher.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

UserService::UserService(UserRepository& userRepo, BookRepository& bookRepo) : userRepo(userRepo), bookRepo(bookRepo)
{}

void UserService::search(const std::string & query) const
{
	auto userMatches = userRepo.fuzzySearchByUsername(query);
	auto bookMatches = bookRepo.fuzzySearchByTitle(query);

    std::cout << "Users:\n";
    if (userMatches.empty()) {
        std::cout << " no users found\n";
    }
    else {
        for (const User* u : userMatches) {
            std::cout << "  " << u->getUsername() << " -> " << u->getType() << "\n";
        }
    }

    std::cout << "\nBooks:\n";
    if (bookMatches.empty()) {
        std::cout << " no books found\n";
    }
    else {
        for (const Book* b : bookMatches) {
            std::cout << "  " << b->getTitle() << " -> " << b->getAvgRating() << "\n";
        }
    }
}

void UserService::followUser(const std::string& fromUsername, const std::string& targetUsername)
{
    if(fromUsername == targetUsername) {
		throw std::invalid_argument("You cannot follow yourself!");
	}

	User* targetUser = userRepo.getMutable(targetUsername);
    if (targetUser == nullptr) {
        throw std::runtime_error("User " + targetUsername + " not found!");
	}

	User* fromUser = userRepo.getMutable(fromUsername);
    if (fromUser->isFollowing(targetUsername)) {
		throw std::invalid_argument("You are already following this user!");
    }

	fromUser->addFollower(targetUsername);

	targetUser->receiveMessage(Message(fromUsername, targetUsername, fromUsername + " started following you!", MessageType::FOLLOW_NOTIFY));
}

std::vector<const User*> UserService::getFriends(const std::string requesterUsername, std::optional<std::string> targetUsername) const
{
	const std::string& target = targetUsername.has_value() ? targetUsername.value() : requesterUsername;

    if(!userRepo.findByUsername(target)) {
        throw std::runtime_error("User " + target + " not found!");
	}

    if (targetUsername.has_value()) {
        auto friends = userRepo.getFriendsOf(requesterUsername);
        bool areFriends = std::any_of(friends.begin(), friends.end(),
            [&targetUsername](const User* u) {
                return u->getUsername() == targetUsername.value();
            });

        if (!areFriends) {
            throw std::runtime_error("You are not friends with " + targetUsername.value() + "!");
        }
    }

    return userRepo.getFriendsOf(target);
}

void UserService::showProfile(const std::string& requesterUsername, std::optional<std::string> targetUsername) const
{
    const std::string& target = targetUsername.has_value() ? targetUsername.value() : requesterUsername;
    const User* user = userRepo.findByUsername(target);
    if (user == nullptr) {
        throw std::runtime_error("User " + target + " not found!");
    }

	user->showProfile();
}

void UserService::addBirthday(const std::string& username, const Date& birthday)
{
	User* user = userRepo.getMutable(username);

    if (user == nullptr) {
        throw std::runtime_error("User " + username + " not found!");
    }

	Reader* reader = dynamic_cast<Reader*>(user);
    if (reader == nullptr) {
        throw std::runtime_error("Only readers and authors can set a birthday!");
	}

	reader->setBirthday(birthday);
}

void UserService::clearBirthday(const std::string& username)
{
    User* user = userRepo.getMutable(username);
    if (user == nullptr) {
        throw std::runtime_error("User " + username + " not found!");
    }

    Reader* reader = dynamic_cast<Reader*>(user);
    if (reader == nullptr) {
        throw std::runtime_error("Only readers and authors can clear a birthday!");
    }

    if(!reader->hasBirthday()) {
        throw std::runtime_error("No birthday set for user " + username + "!");
	}

	reader->clearBirthday();
}

void UserService::showInbox(const std::string& username, std::optional<MessageType> filter) const
{
	const User* user = userRepo.findByUsername(username);
    if (user == nullptr) {
        throw std::runtime_error("User " + username + " not found!");
	}

	const auto& inbox = user->getInbox();
    if(inbox.empty()) {
        std::cout << "Inbox is empty!\n";
        return;
	}

	bool onePrinted = false;
    for (size_t i = 0; i < inbox.size(); ++i) {
        const Message& msg = inbox[i];
        if (filter.has_value() && msg.getType() != filter.value()) {
			continue;
        }
            
        printMessage(i, msg);
        onePrinted = true;
    }

    if (!onePrinted) {
        std::cout << "No messages of the specified type!\n";
	}
}

void UserService::readMessage(const std::string& username, int messageIndex)
{
	User* user = userRepo.getMutable(username);

    if (user == nullptr) {
        throw std::runtime_error("User " + username + " not found!");
    }

	Message* msg = user->getMessageAt(messageIndex);
    if (msg == nullptr) {
        throw std::runtime_error("Message index out of bounds!");
	}

    if(msg->isMessageRead()) {
		throw std::runtime_error("Message already read!");
    }

	msg->markAsRead();
}

void UserService::deleteMessage(const std::string& username, int messageIndex)
{
	User* user = userRepo.getMutable(username);

    if (user == nullptr) {
        throw std::runtime_error("User " + username + " not found!");
    }

	Message* msg = user->getMessageAt(messageIndex);
    if (msg == nullptr) {
        throw std::runtime_error("Message index out of bounds!");
	}

	if (!msg->isMessageRead()) {
        throw std::runtime_error("Cannot delete an unread message!");
	}

	user->deleteMessage(messageIndex);
}

void UserService::acceptJobOffer(const std::string& username, int messageIndex)
{
    User* user = userRepo.getMutable(username);
    if (user == nullptr) {
        throw std::runtime_error("User " + username + " not found!");
    }

	Author* author = dynamic_cast<Author*>(user);
    if(author == nullptr) {
        throw std::runtime_error("Only authors can accept job offers!");
	}

    Message* msg = author->getMessageAt(messageIndex);
    if (msg == nullptr) {
        throw std::runtime_error("Message index out of bounds!");
    }
    if (msg->getType() != MessageType::JOB_OFFER) {
        throw std::runtime_error("Message is not a job offer!");
    }

	const std::string& publisherUsername = msg->getNameSender();

	User* publisherUser = userRepo.getMutable(publisherUsername);
    if (publisherUser == nullptr) {
        throw std::runtime_error("Publisher user " + publisherUsername + " not found!");
	}

	Publisher* publisher = dynamic_cast<Publisher*>(publisherUser);
    if(publisher == nullptr) {
        throw std::runtime_error("Sender of the job offer is not a publisher!");
	}


	author->addPublisher(publisherUsername);
	publisher->addAuthor(author->getUsername());
	msg->markAsRead();
}

void UserService::leaveJob(const std::string& authorUsername, const std::string& publisherUsername)
{
    User* authorUser = userRepo.getMutable(authorUsername);
    if (authorUser == nullptr) {
        throw std::runtime_error("Author user " + authorUsername + " not found!");
    }

    Author* author = dynamic_cast<Author*>(authorUser);
    if (author == nullptr) {
        throw std::runtime_error("User " + authorUsername + " is not an author!");
    }

    User* publisherUser = userRepo.getMutable(publisherUsername);
    if (publisherUser == nullptr) {
        throw std::runtime_error("Publisher user " + publisherUsername + " not found!");
    }

    Publisher* publisher = dynamic_cast<Publisher*>(publisherUser);
    if (publisher == nullptr) {
        throw std::runtime_error("User " + publisherUsername + " is not a publisher!");
    }

    if (!author->hasPublisher(publisherUsername)) {
        throw std::runtime_error("Author " + authorUsername + " does not work for publisher " + publisherUsername + "!");
    }


    author->removePublisher(publisherUsername);
	publisher->removeAuthor(authorUsername);
}

void UserService::showFollowers(const std::string& authorUsername) const
{
    User* authorUser = userRepo.getMutable(authorUsername);
    if (authorUser == nullptr) {
        throw std::runtime_error("Author user " + authorUsername + " not found!");
    }

    Author* author = dynamic_cast<Author*>(authorUser);
    if (author == nullptr) {
        throw std::runtime_error("User " + authorUsername + " is not an author!");
    }

    const auto& followers = userRepo.getFollowersOf(authorUsername);
    if(followers.empty()) {
        std::cout << "No followers!\n";
        return;
    }

    std::cout << "Followers of " << authorUsername << ":\n";
    for (const User* follower : followers) {
        std::cout << "  " << follower->getUsername() << " -> " << follower->getType() << "\n";
	}
}

void UserService::sendOffer(const std::string& publisherUsername, const std::string& authorUsername)
{
	const User* publisherUser = userRepo.findByUsername(publisherUsername);
    if(publisherUser == nullptr) {
        throw std::runtime_error("Publisher user " + publisherUsername + " not found!");
    }

    const Publisher* publisher = dynamic_cast<const Publisher*>(publisherUser);
    if (publisher == nullptr) {
        throw std::runtime_error("User " + publisherUsername + " is not a publisher!");
	}


	User* authorUser = userRepo.getMutable(authorUsername);
    if (authorUser == nullptr) {
        throw std::runtime_error("Author user " + authorUsername + " not found!");
    }

    Author* author = dynamic_cast<Author*>(authorUser);
    if (author == nullptr) {
        throw std::runtime_error("User " + authorUsername + " is not an author!");
	}

	author->receiveMessage(Message(publisherUsername, authorUsername, publisherUsername + " has sent you a job offer!", MessageType::JOB_OFFER));
}

void UserService::printMessage(int index, const Message& msg) const
{
    std::cout << "[" << index << "] "
        << (msg.isMessageRead() ? "[read]  " : "[unread]")
        << " From: " << msg.getNameSender()
        << " | " << msg.getContent() << "\n";
}