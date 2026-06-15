#include "commands/AddBookCommand.h"
#include "models/PersonalLibrary.h"
#include <iostream>

AddBookCommand::AddBookCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

ReadStatus AddBookCommand::statusToString(const std::string& status)
{
    if (status == "plan-to-read") return ReadStatus::PLAN_TO_READ;
    if (status == "reading")      return ReadStatus::READING;
    if (status == "paused")       return ReadStatus::PAUSED;
    if (status == "dropped")      return ReadStatus::DROPPED;
    throw std::invalid_argument("Invalid status!");
}

void AddBookCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: you must be logged in as a reader!\n";
        return;
    }

    if (data.size() < 2) {
        std::cout << "Correct method usage: add-book <bookName> <status> [rating]\n";
        return;
    }

    try {
        ReadStatus status = statusToString(data[1]);

        int rating = -1;
        if (data.size() >= 3) {
            rating = std::stoi(data[2]);
        }

        bookService.addBook(reader, data[0], status, rating);
        std::cout << "Book added successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}