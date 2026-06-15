#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/BookService.h"

class DeleteShelfCommand : public Command
{
private:

    AuthService& authService;
    BookService& bookService;

public:

    DeleteShelfCommand(AuthService& authService, BookService& bookService);
    void execute(const std::vector<std::string>& data) override;

};
