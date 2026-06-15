#include "commands/ProfileCommand.h"
#include <iostream>
#include <optional>

ProfileCommand::ProfileCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void ProfileCommand::execute(const std::vector<std::string>& data)
{
    User* user = authService.getCurrentUser();
    if (user == nullptr) {
        std::cout << "Error: you must be logged in to view a profile!\n";
        return;
    }

    try {
        std::optional<std::string> targetUsername = std::nullopt;
        if (data.size() == 1) {
            targetUsername = data[0];
        }

        const User* profile = userService.getProfile(user, targetUsername);
        if (profile == nullptr) {
            std::cout << "User not found!\n";
            return;
        }

        profile->showProfile();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}