#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/BookService.h"
#include "repositories/BookRepository.h"

class ShowShelfCommand : public Command
{
private:

    AuthService& authService;
    BookService& bookService;
    BookRepository& bookRepo;

public:

    ShowShelfCommand(AuthService& authService, BookService& bookService, BookRepository& bookRepo);
    void execute(const std::vector<std::string>& data) override;

};