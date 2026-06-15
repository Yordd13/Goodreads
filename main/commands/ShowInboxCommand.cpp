#include "commands/ShowInboxCommand.h"
#include "models/Message.h"
#include <iostream>
#include <optional>

ShowInboxCommand::ShowInboxCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void ShowInboxCommand::execute(const std::vector<std::string>& data)
{
    User* user = authService.getCurrentUser();
    if (user == nullptr) {
        std::cout << "Error: you must be logged in to view your inbox!\n";
        return;
    }

    std::optional<MessageType> filter = std::nullopt;

    if (data.size() >= 1) {
        const std::string& filterArg = data[0];

        if (filterArg == "follow") {
            filter = MessageType::FOLLOW_NOTIFY;
        }
        else if (filterArg == "offers") {
            filter = MessageType::JOB_OFFER;
        }
        else {
            std::cout << "Unknown filter!\n";
            return;
        }
    }

    try {
        auto messages = userService.getInbox(user, filter);

        if (messages.empty()) {
            std::cout << "Your inbox is empty!\n";
            return;
        }

        std::cout << "Inbox (" << messages.size() << " messages):\n";
        for (size_t i = 0; i < messages.size(); i++) {
            const Message* msg = messages[i];
            std::string readStatus = msg->isMessageRead() ? "[read]" : "[unread]";
            std::cout << "[" << i << "] " << readStatus << " From " << msg->getNameSender() << ": " << msg->getContent() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}