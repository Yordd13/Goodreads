#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/BookService.h"

class RemoveFromShelfCommand : public Command
{
private:

    AuthService& authService;
    BookService& bookService;

public:

    RemoveFromShelfCommand(AuthService& authService, BookService& bookService);
    void execute(const std::vector<std::string>& data) override;

};