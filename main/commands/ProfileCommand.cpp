#include "commands/ProfileCommand.h"
#include <iostream>
#include <optional>

ProfileCommand::ProfileCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void ProfileCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers and authors can view their profiles!\n";
        return;
    }

    try {
        std::optional<std::string> targetUsername = std::nullopt;
        if (data.size() == 1) {
            targetUsername = data[0];
        }

        const User* profile = userService.getProfile(reader, targetUsername);
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