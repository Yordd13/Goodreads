#pragma once
#include <string>
#include <vector>
#include "repositories/Repository.hpp"
#include "users/User.h"

class UserRepository : public Repository<User>
{
private:
    std::string filePath;

public:
    UserRepository(const std::string& filePath);

    ~UserRepository();

    const User* findByUsername(const std::string& username) const;

    bool usernameExists(const std::string& username) const;

    std::vector<const User*> getFollowersOf(const std::string& username) const;

    std::vector<const User*> getFriendsOf(const std::string& username) const;

    User* getMutable(const std::string& username);

    std::vector<User*> getAllMutable();

private:
    void loadFromFile();
    void saveToFile() const;
};