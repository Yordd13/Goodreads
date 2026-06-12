#include "commands/FollowersCommand.h"
#include <iostream>

FollowersCommand::FollowersCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void FollowersCommand::execute(const std::vector<std::string>& data)
{
    Author* author = authService.getCurrentAuthor();
    if (author == nullptr) {
        std::cout << "Error: only authors can view their followers!\n";
        return;
    }

    try {
        auto followers = userService.getFollowers(author);

        if (followers.empty()) {
            std::cout << "You have no followers!\n";
            return;
        }

        std::cout << "Your followers (" << followers.size() << "):\n";
        for (const User* f : followers) {
            std::cout << "  " << f->getUsername() << " -> " << f->getType() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}