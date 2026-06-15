#include "commands/ShowShelfCommand.h"
#include "models/Shelf.h"
#include "models/Book.h"
#include <iostream>
#include <optional>

ShowShelfCommand::ShowShelfCommand(AuthService& authService, BookService& bookService, BookRepository& bookRepo) : authService(authService), bookService(bookService), bookRepo(bookRepo)
{}

void ShowShelfCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers and authors can show shelves!\n";
        return;
    }

    if (data.size() < 1) {
        std::cout << "Correct method usage: show-shelf [reader] <name>\n";
        return;
    }

    try {
        std::optional<std::string> targetUsername = std::nullopt;
        std::string shelfName;

        if (data.size() >= 2) {
            targetUsername = data[0];
            shelfName = data[1];
        }
        else {
            shelfName = data[0];
        }

        const Shelf* shelf = bookService.getShelf(reader, shelfName, targetUsername);
        if (shelf == nullptr) {
            std::cout << "Shelf '" << shelfName << "' not found!\n";
            return;
        }

        std::cout << "Shelf: " << shelf->getName() << " (" << shelf->getBookCount() << " books)\n";

        for (const std::string& title : shelf->getBooks()) {
            const Book* book = bookRepo.findByTitle(title);
            if (book != nullptr) {
                std::cout << "  " << book->getTitle() << " -> " << book->getAvgRating() << "\n";
            }
            else {
                std::cout << "  " << title << "\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}