#include "commands/DeleteBookCommand.h"
#include <iostream>

DeleteBookCommand::DeleteBookCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

void DeleteBookCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: you must be logged in as a reader!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: delete-book <bookName>\n";
        return;
    }

    try {
        bookService.deleteBook(reader, data[0]);
        std::cout << "Book was removed from your profile!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}