#include "commands/DeleteShelfCommand.h"
#include <iostream>

DeleteShelfCommand::DeleteShelfCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

void DeleteShelfCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers and authors can delete shelves!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: delete-shelf <name>\n";
        return;
    }

    try {
        bookService.deleteShelf(reader, data[0]);
        std::cout << "Shelf deleted successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}