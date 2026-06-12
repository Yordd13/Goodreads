#include "commands/AddSynopsisCommand.h"
#include <iostream>

AddSynopsisCommand::AddSynopsisCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

void AddSynopsisCommand::execute(const std::vector<std::string>& data)
{
    Publisher* publisher = authService.getCurrentPublisher();
    if (publisher == nullptr) {
        std::cout << "Error: only publishers can add a synopsis!\n";
        return;
    }

    if (data.size() < 2) {
        std::cout << "Correct method usage: add-synopsis <bookTitle> <synopsis>\n";
        return;
    }

    try {
        std::string synopsis;
        for (size_t i = 1; i < data.size(); i++) {
            if (i > 1) {
                synopsis += " ";
            }
            synopsis += data[i];
        }

        bookService.addSynopsis(publisher, data[0], synopsis);
        std::cout << "Synopsis added successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}