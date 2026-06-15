#include "commands/ReadMsgCommand.h"
#include <iostream>

ReadMsgCommand::ReadMsgCommand(AuthService& authService, UserService& userService)
    : authService(authService), userService(userService)
{
}

void ReadMsgCommand::execute(const std::vector<std::string>& data)
{
    User* user = authService.getCurrentUser();
    if (user == nullptr) {
        std::cout << "Error: you must be logged in to read messages!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: read-msg <index>\n";
        return;
    }

    try {
        int index = std::stoi(data[0]);
        userService.readMessage(user, index);
        std::cout << "Message was read!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}