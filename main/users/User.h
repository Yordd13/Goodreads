#pragma once
#include "models/Message.h"
#include <string>
#include <vector>
#include "models/Date.h"

class User
{
    friend class UserRepository;

private:
    std::string username;
    std::string password;
    Date registrationDate;
    std::vector<std::string> followerUsernames;

    void setUsername(const std::string& username);
    void setPassword(const std::string& password);

protected:

    std::vector<Message> inbox;

    void serializeBase(std::ostream& out) const;
    void deserializeBase(std::istream& in);

    virtual void serializeExtra(std::ostream& out) const = 0;
    virtual void deserializeExtra(std::istream& in) = 0;

public:
    User() = default;
    User(const std::string& username, const std::string& password, const Date& registrationDate);

    std::string getUsername() const;
    Date getRegistrationDate() const;
    const std::vector<std::string>& getFollowers() const;

    void addFollower(const std::string& username);
    bool isFollowedBy(const std::string& username) const;
    bool checkPassword(const std::string& password) const;

    void receiveMessage(const Message& msg);
    Message* getMessageAt(int index);
    bool deleteMessage(int index);
    const std::vector<Message>& getInbox() const;

    virtual std::string getType() const = 0;
    virtual void showProfile() const = 0;

    virtual ~User() = default;

    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
};