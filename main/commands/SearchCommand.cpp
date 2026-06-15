#include "commands/SearchCommand.h"
#include <iostream>

SearchCommand::SearchCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void SearchCommand::execute(const std::vector<std::string>& data)
{
    if (authService.getCurrentUser() == nullptr) {
        std::cout << "Error: you must be logged in to search!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: search <name>\n";
        return;
    }

    try {
        auto [users, books] = userService.search(data[0]);

        std::cout << "Users:\n";
        if (users.empty()) {
            std::cout << "No users found.\n";
        }
        else {
            for (const User* u : users) {
                std::cout << u->getUsername() << " -> " << u->getType() << "\n";
            }
        }

        std::cout << "\nBooks:\n";
        if (books.empty()) {
            std::cout << "No books found.\n";
        }
        else {
            for (const Book* b : books) {
                std::cout << b->getTitle() << " -> " << b->getAvgRating() << "\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}