#include "commands/AcceptOfferCommand.h"
#include <iostream>

AcceptOfferCommand::AcceptOfferCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void AcceptOfferCommand::execute(const std::vector<std::string>& data)
{
    Author* author = authService.getCurrentAuthor();
    if (author == nullptr) {
        std::cout << "Error: only authors can accept job offers!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: accept-offer <index>\n";
        return;
    }

    try {
        int index = std::stoi(data[0]);
        userService.acceptJobOffer(author, index);
        std::cout << "Job offer accepted successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}