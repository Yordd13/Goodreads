#include "services/AuthService.h"
#include "validator/UserValidation.h"
#include <stdexcept>
#include <algorithm>


AuthService::AuthService(UserRepository& userRepo) : userRepo(userRepo), currentUser(nullptr)
{}

void AuthService::validateRegistration(const std::string& username, const std::string& password, const std::string& type) const
{
    if (!userValidation::isValidUsername(username)) {
        throw std::invalid_argument("Invalid username! Must be between 6 and 24 characters!");
    }

    if (!userValidation::isValidPassword(password)) {
        throw std::invalid_argument("Invalid password! Must be 12-36 characters and contain "
            "at least one lowercase letter, one uppercase letter, "
            "and one non-letter character!");
    }

    if (userRepo.usernameExists(username)) {
        throw std::runtime_error("Username is already taken!");
    }

    std::string lower = type;
    std::transform(lower.begin(), lower.end(), lower.begin(),
        [](unsigned char c) { return std::tolower(c); });

    if (lower != "reader" && lower != "author" && lower != "publisher") {
        throw std::invalid_argument("Unknown user type! Must be reader, author, or publisher!");
    }
}

std::unique_ptr<User> AuthService::createUser(const std::string& username, const std::string& password, const std::string& type) const
{
    Date today = DateUtils::today();

    std::string lower = type;
    std::transform(lower.begin(), lower.end(), lower.begin(),
        [](unsigned char c) { return std::tolower(c); });

    if (lower == "reader")    return std::make_unique<Reader>(username, password, today);
    if (lower == "author")    return std::make_unique<Author>(username, password, today);
    if (lower == "publisher") return std::make_unique<Publisher>(username, password, today);

    throw std::invalid_argument("Unknown user type!");
}

void AuthService::registerUser(const std::string& username, const std::string& password, const std::string& type)
{
    validateRegistration(username, password, type);
    userRepo.add(createUser(username, password, type));
}

void AuthService::login(const std::string& username, const std::string& password)
{
    if (isLoggedIn()) {
        throw std::runtime_error("Already logged in as " + getCurrentUsername() + "!");
    }
    User* user = userRepo.getMutable(username);
    if (user == nullptr) {
        throw std::runtime_error("User not found!");
    }

    if (!user->checkPassword(password)) {
        throw std::invalid_argument("Incorrect password!");
    }

    currentUser = user;
}

void AuthService::logout()
{
    if (!isLoggedIn()) {
        throw std::runtime_error("No user is currently logged in!");
    }

    currentUser = nullptr;
}

bool AuthService::isLoggedIn() const
{
    return currentUser != nullptr;
}

User* AuthService::getCurrentUser() const
{
    return currentUser;
}

std::string AuthService::getCurrentUsername() const
{
    if (!isLoggedIn()) {
        return "";
    }
    return currentUser->getUsername();
}

std::string AuthService::getCurrentUserType() const
{
    if (!isLoggedIn()) {
        return "";
    }
    return currentUser->getType();
}

Reader* AuthService::getCurrentReader() const
{
    return dynamic_cast<Reader*>(currentUser);
}

Author* AuthService::getCurrentAuthor() const
{
    return dynamic_cast<Author*>(currentUser);
}

Publisher* AuthService::getCurrentPublisher() const
{
    return dynamic_cast<Publisher*>(currentUser);
}
