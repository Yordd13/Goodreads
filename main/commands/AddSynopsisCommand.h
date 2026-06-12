#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/BookService.h"

class AddSynopsisCommand : public Command
{
private:

    AuthService& authService;
    BookService& bookService;

public:

    AddSynopsisCommand(AuthService& authService, BookService& bookService);
    void execute(const std::vector<std::string>& data) override;

};

