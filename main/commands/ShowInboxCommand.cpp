#include "commands/ShowInboxCommand.h"
#include "models/Message.h"
#include <iostream>
#include <optional>

ShowInboxCommand::ShowInboxCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void ShowInboxCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers and authors can view the inbox\n";
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
        const std::vector<Message>& inbox = reader->getInbox();

        bool anyFound = false;
        std::cout << "Inbox:\n";

        for (int i = 0; i < inbox.size(); i++) {
            const Message& msg = inbox[i];
            if (filter.has_value() && msg.getType() != filter.value()) {
                continue;
            }
            anyFound = true;
            std::string readStatus = msg.isMessageRead() ? "[read]" : "[unread]";
            std::cout << "[" << i << "] " << readStatus << " From " << msg.getNameSender() << ": " << msg.getContent() << "\n";
        }

        if (!anyFound) {
            std::cout << "Your inbox is empty!\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}