#include "commands/RegisterCommand.h"
#include <iostream>

RegisterCommand::RegisterCommand(AuthService& authService) : authService(authService)
{}

void RegisterCommand::execute(const std::vector<std::string>& data)
{
    if (data.size() < 3) {
        std::cout << "Correct method usage:  register <username> <password> <type>\n";
        return;
    }

    try {
        authService.registerUser(data[0], data[1], data[2]);
        std::cout << "User " << data[0] << " registered successfully.\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}