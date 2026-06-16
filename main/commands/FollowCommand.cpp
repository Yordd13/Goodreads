#include "commands/FollowCommand.h"
#include <iostream>

FollowCommand::FollowCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void FollowCommand::execute(const std::vector<std::string>& data)
{
    Reader* user = authService.getCurrentReader();
    if (user == nullptr) {
        std::cout << "Error: only readers and authors can follow!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: follow <username>\n";
        return;
    }

    try {
        userService.followUser(user, data[0]);
        std::cout << "You are now following " << data[0] << "!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}