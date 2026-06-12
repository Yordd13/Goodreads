#include "commands/LeaveJobCommand.h"
#include <iostream>

LeaveJobCommand::LeaveJobCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void LeaveJobCommand::execute(const std::vector<std::string>& data)
{
    Author* author = authService.getCurrentAuthor();
    if (author == nullptr) {
        std::cout << "Error: only authors can leave a publisher!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: leave <publisher>\n";
        return;
    }

    try {
        userService.leaveJob(author, data[0]);
        std::cout << "You left the publisher!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}