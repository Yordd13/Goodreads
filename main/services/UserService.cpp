#include "services/UserService.h"
#include "users/Reader.h"
#include "users/Author.h"
#include "users/Publisher.h"
#include <algorithm>
#include <stdexcept>

UserService::UserService(UserRepository& userRepo, BookRepository& bookRepo) : userRepo(userRepo), bookRepo(bookRepo)
{}

std::pair<std::vector<const User*>, std::vector<const Book*>> UserService::search(const std::string& query) const
{
    return {
        userRepo.fuzzySearchByUsername(query),
        bookRepo.fuzzySearchByTitle(query)
    };
}

void UserService::followUser(User* currUser, const std::string& targetUsername)
{
    if(currUser->getUsername() == targetUsername) {
		throw std::invalid_argument("You cannot follow yourself!");
	}

	User* targetUser = userRepo.getMutable(targetUsername);
    if (targetUser == nullptr) {
        throw std::runtime_error("User " + targetUsername + " not found!");
	}

    if (currUser->isFollowing(targetUsername)) {
		throw std::invalid_argument("You are already following this user!");
    }

	currUser->addFollower(targetUsername);
	targetUser->receiveMessage(Message(currUser->getUsername(), targetUsername, currUser->getUsername() + " started following you!", MessageType::FOLLOW_NOTIFY));
}

std::vector<const User*> UserService::getFriends(User* currUser, std::optional<std::string> targetUsername) const
{
    if (!targetUsername.has_value()) {
        return userRepo.getFriendsOf(currUser->getUsername());
    }

    const std::string& target = targetUsername.value();

    if(!userRepo.findByUsername(target)) {
        throw std::runtime_error("User " + target + " not found!");
	}

    auto friends = userRepo.getFriendsOf(currUser->getUsername());
    bool areFriends = std::any_of(friends.begin(), friends.end(),
        [&target](const User* u) { return u->getUsername() == target; });


    if (!areFriends) {
        throw std::runtime_error("You are not friends with " + target + "!");
    }

    return userRepo.getFriendsOf(target);
}

const User* UserService::getProfile(User* currUser, std::optional<std::string> targetUsername) const
{
    if (!targetUsername.has_value()) {
        return currUser;
    }

    const User* target = userRepo.findByUsername(targetUsername.value());
    if (!target){
        throw std::runtime_error("User " + targetUsername.value() + " not found!");
    }

    return target;
}

void UserService::addBirthday(Reader* currReader, const Date& birthday)
{
    currReader->setBirthday(birthday);
}

void UserService::clearBirthday(Reader* currReader)
{
    if(currReader->hasBirthday()) {
        currReader->clearBirthday();
    }
}

std::vector<const Message*> UserService::getInbox(User* currUser, std::optional<MessageType> filter) const
{
    std::vector<const Message*> result;
    for (const Message& msg : currUser->getInbox()) {
        if (!filter.has_value() || msg.getType() == filter.value()) {
            result.push_back(&msg);
        }
    }
    return result;
}

void UserService::readMessage(User* currUser, int messageIndex)
{
    Message* msg = currUser->getMessageAt(messageIndex);
    if (msg == nullptr) {
        throw std::out_of_range("Message index out of bounds!");
    }

    if (msg->isMessageRead()) {
        throw std::invalid_argument("Message is already read!");
    }

    msg->markAsRead();
}

void UserService::deleteMessage(User* currUser, int messageIndex)
{   

	Message* msg = currUser->getMessageAt(messageIndex);
    if (msg == nullptr) {
        throw std::out_of_range("Message index out of bounds!");
	}

	if (!msg->isMessageRead()) {
        throw std::runtime_error("Cannot delete an unread message!");
	}

	currUser->deleteMessage(messageIndex);
}

void UserService::acceptJobOffer(Author* currAuthor, int messageIndex)
{
    Message* msg = currAuthor->getMessageAt(messageIndex);
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


    currAuthor->addPublisher(publisherUsername);
	publisher->addAuthor(currAuthor->getUsername());
	msg->markAsRead();
}

void UserService::leaveJob(Author* currAuthor, const std::string& publisherUsername)
{
    User* publisherUser = userRepo.getMutable(publisherUsername);
    if (publisherUser == nullptr) {
        throw std::runtime_error("Publisher user " + publisherUsername + " not found!");
    }

    Publisher* publisher = dynamic_cast<Publisher*>(publisherUser);
    if (publisher == nullptr) {
        throw std::runtime_error("User " + publisherUsername + " is not a publisher!");
    }

    if (!currAuthor->hasPublisher(publisherUsername)) {
        throw std::runtime_error("Author " + currAuthor->getUsername() + " does not work for publisher " + publisherUsername + "!");
    }


    currAuthor->removePublisher(publisherUsername);
	publisher->removeAuthor(currAuthor->getUsername());
}

std::vector<const User*> UserService::getFollowers(Author* currAuthor) const
{
	return userRepo.getFollowersOf(currAuthor->getUsername());
}

void UserService::sendOffer(Publisher* currPublisher, const std::string& authorUsername)
{
    User* authorUser = userRepo.getMutable(authorUsername);
    if (authorUser == nullptr) {
        throw std::runtime_error("Author user " + authorUsername + " not found!");
    }

    Author* author = dynamic_cast<Author*>(authorUser);
    if (author == nullptr) {
        throw std::runtime_error("User " + authorUsername + " is not an author!");
	}

	author->receiveMessage(Message(currPublisher->getUsername(), authorUsername, currPublisher->getUsername() + " has sent you a job offer!", MessageType::JOB_OFFER));
}