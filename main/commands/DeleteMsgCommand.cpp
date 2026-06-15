#include "commands/DeleteMsgCommand.h"
#include <iostream>

DeleteMsgCommand::DeleteMsgCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void DeleteMsgCommand::execute(const std::vector<std::string>& data)
{
    User* user = authService.getCurrentUser();
    if (user == nullptr) {
        std::cout << "Error: you must be logged in to delete messages!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: delete-msg <index>\n";
        return;
    }

    try {
        int index = std::stoi(data[0]);
        userService.deleteMessage(user, index);
        std::cout << "Message deleted successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}