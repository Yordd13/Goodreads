#include "commands/RemoveFromShelfCommand.h"
#include <iostream>

RemoveFromShelfCommand::RemoveFromShelfCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

void RemoveFromShelfCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers and authors can remove books from shelves\n";
        return;
    }

    if (data.size() < 2) {
        std::cout << "Correct method usage: remove-from-shelf <bookName> <shelfName>\n";
        return;
    }

    try {
        bookService.removeFromShelf(reader, data[0], data[1]);
        std::cout << "Book removed from shelf successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}