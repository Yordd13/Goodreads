#include "commands/PublishCommand.h"
#include "validator/DateValidation.h"
#include <iostream>

PublishCommand::PublishCommand(AuthService& authService, BookService& bookService) : authService(authService), bookService(bookService)
{}

void PublishCommand::execute(const std::vector<std::string>& data)
{
    Publisher* publisher = authService.getCurrentPublisher();
    if (publisher == nullptr) {
        std::cout << "Error: only publishers can publish books!\n";
        return;
    }

    if (data.size() < 5) {
        std::cout << "Correct method usage: publish <bookTitle> <authorName> <releaseDate> <pageCount> <genres...>\n";
        return;
    }

    try {
        Date releaseDate = Date::fromString(data[2]);
        size_t pageCount = std::stoul(data[3]);
        std::vector<std::string> genres(data.begin() + 4, data.end());

        bookService.publish(publisher, data[0], data[1], releaseDate, pageCount, genres);

        std::cout << "Book published successfully!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}