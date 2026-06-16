#include "users/User.h"
#include "validator/UserValidation.h"
#include <stdexcept>
#include <iostream>

void User::setUsername(const std::string& username){
    if(!userValidation::isValidUsername(username)) {
        throw std::invalid_argument("Invalid username");
    }

    this->username = username;
}

void User::setPassword(const std::string& password){
    if(!userValidation::isValidPassword(password)) {
        throw std::invalid_argument("Invalid password");
    }

	this->password = password;
}

User::User(const std::string& username, const std::string& password, const Date& registrationDate) : registrationDate(registrationDate) {
    setUsername(username);
    setPassword(password);
}

std::string User::getUsername() const
{
    return username;
}

Date User::getRegistrationDate() const
{
    return registrationDate;
}

const std::vector<std::string>& User::getFollowers() const
{
    return followerUsernames;
}

void User::addFollower(const std::string& username)
{ 
    if(!isFollowedBy(username)) {
        followerUsernames.push_back(username);
    }
}

bool User::isFollowedBy(const std::string& username) const
{
    return std::find(followerUsernames.begin(), followerUsernames.end(), username) != followerUsernames.end();
}

bool User::checkPassword(const std::string& password) const
{
    return this->password == password;
}

void User::receiveMessage(const Message& msg)
{
    inbox.push_back(msg);
}

Message* User::getMessageAt(int index)
{
    if (index < 0 || index >= inbox.size()) {
        return nullptr;
    }

    return &inbox[index];
}

bool User::deleteMessage(int index)
{
    if (index < 0 || index >= inbox.size()) {
        return false;
    }

    if (!inbox[index].isMessageRead()) {
        return false;
    }

    inbox.erase(inbox.begin() + index);
    return true;
}

const std::vector<Message>& User::getInbox() const
{
    return inbox;
}

void User::serializeBase(std::ostream& out) const
{
    out << username << '\n';
    out << password << '\n';
    out << registrationDate.toSerial() << '\n';
    out << followerUsernames.size() << '\n';
    
    for (const auto& f : followerUsernames) {
        out << f << '\n';
    }

    out << inbox.size() << '\n';
    for (const auto& msg : inbox) {
        msg.toSerial(out);
    }
}

void User::deserializeBase(std::istream& in)
{
    std::getline(in, username);
    std::getline(in, password);

    int serialDate;
    in >> serialDate;
    in.ignore();
    registrationDate = Date::fromSerial(serialDate);

    size_t followerCount;
    in >> followerCount;
    in.ignore();

    followerUsernames.clear();
    followerUsernames.reserve(followerCount);
    for (size_t i = 0; i < followerCount; i++){
        std::string f;
        std::getline(in, f);
        followerUsernames.push_back(f);
    }

    size_t inboxCount;
    in >> inboxCount;
    in.ignore();

    inbox.clear();
    for (size_t i = 0; i < inboxCount; i++) {
        inbox.push_back(Message::fromSerial(in));
    }
}

void User::serialize(std::ostream& out) const
{
    out << getType() << '\n';
    serializeBase(out);
    serializeExtra(out);
}

void User::deserialize(std::istream& in)
{
    deserializeBase(in);
	deserializeExtra(in);
}
