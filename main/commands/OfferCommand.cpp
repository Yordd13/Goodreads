#include "commands/OfferCommand.h"
#include <iostream>

OfferCommand::OfferCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void OfferCommand::execute(const std::vector<std::string>& data)
{
    Publisher* publisher = authService.getCurrentPublisher();
    if (publisher == nullptr) {
        std::cout << "Error: only publishers can send job offers!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: offer <author>\n";
        return;
    }

    try {
        userService.sendOffer(publisher, data[0]);
        std::cout << "Job offer sent to " << data[0] << "!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}