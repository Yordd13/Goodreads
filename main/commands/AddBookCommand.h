#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/BookService.h"

class AddBookCommand : public Command
{
private:

    AuthService& authService;
    BookService& bookService;

    static ReadStatus statusToString(const std::string& status);

public:

    AddBookCommand(AuthService& authService, BookService& bookService);
    void execute(const std::vector<std::string>& data) override;

};

