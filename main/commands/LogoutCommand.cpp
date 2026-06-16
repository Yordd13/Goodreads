#include "commands/LogoutCommand.h"
#include <iostream>

LogoutCommand::LogoutCommand(AuthService& authService) : authService(authService)
{}

void LogoutCommand::execute(const std::vector<std::string>& data)
{
    try {
        authService.logout();
        std::cout << "Successfully logged out!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}