#include "commands/AddToShelfCommand.h"
#include <iostream>

AddToShelfCommand::AddToShelfCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

void AddToShelfCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers can add books to shelves!\n";
        return;
    }

    if (data.size() < 2) {
        std::cout << "Correct method usage: add-to-shelf <bookName> <shelfName>\n";
        return;
    }

    try {
        bookService.addToShelf(reader, data[0], data[1]);
        std::cout << "Book added to shelf successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}