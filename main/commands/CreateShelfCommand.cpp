#include "commands/CreateShelfCommand.h"
#include <iostream>

CreateShelfCommand::CreateShelfCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

void CreateShelfCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers and authors can create shelves!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: create-shelf <name>\n";
        return;
    }

    try {
        bookService.createShelf(reader, data[0]);
        std::cout << "Shelf created successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}