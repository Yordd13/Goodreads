#pragma once
#include <string>
#include "repositories/UserRepository.h"
#include "users/Reader.h"
#include "users/Author.h"
#include "users/Publisher.h"

class AuthService {
private:

	UserRepository& userRepo;
	User* currentUser;

public:

    AuthService(UserRepository& userRepo);

    void registerUser(const std::string& username, const std::string& password, const std::string& type);

    void login(const std::string& username, const std::string& password);

    void logout();

    bool isLoggedIn() const;

    User* getCurrentUser() const;

    std::string getCurrentUsername() const;

    std::string getCurrentUserType() const;

    Reader* getCurrentReader()    const;

    Author* getCurrentAuthor()    const;

    Publisher* getCurrentPublisher() const;

};