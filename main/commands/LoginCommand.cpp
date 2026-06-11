#include "commands/LoginCommand.h"
#include <iostream>

LoginCommand::LoginCommand(AuthService& authService) : authService(authService)
{}

void LoginCommand::execute(const std::vector<std::string>& data)
{
    if (data.size() < 2) {
        std::cout << "Correct method usage:  login <username> <password>\n";
        return;
    }

    try {
        authService.login(data[0], data[1]);
        std::cout << "Successfully logged in as " << data[0] << "!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}