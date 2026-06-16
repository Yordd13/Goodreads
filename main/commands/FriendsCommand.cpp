#include "commands/FriendsCommand.h"
#include <iostream>
#include <optional>

FriendsCommand::FriendsCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void FriendsCommand::execute(const std::vector<std::string>& data)
{
    Reader* user = authService.getCurrentReader();
    if (user == nullptr) {
        std::cout << "Error: only readers and authors can view friends!\n";
        return;
    }

    try {
        std::optional<std::string> targetUsername = std::nullopt;
        if (data.size() >= 1) {
            targetUsername = data[0];
        }

        auto friends = userService.getFriends(user, targetUsername);

        std::string name = targetUsername.has_value() ? targetUsername.value() : user->getUsername();

        if (friends.empty()) {
            std::cout << name << " has no friends!\n";
            return;
        }

        std::cout << "Friends of " << name << " (" << friends.size() << "):\n";
        for (const User* f : friends) {
            std::cout << "  " << f->getUsername() << " -> " << f->getType() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}