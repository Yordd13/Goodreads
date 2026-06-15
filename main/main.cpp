#include "commands/HelpCommand.h"
#include "commands/RegisterCommand.h"
#include "commands/LoginCommand.h"
#include "commands/LogoutCommand.h"
#include "commands/SearchCommand.h"
#include "commands/FollowCommand.h"
#include "commands/AddBookCommand.h"
#include "commands/DeleteBookCommand.h"
#include "commands/CreateShelfCommand.h"
#include "commands/DeleteShelfCommand.h"
#include "commands/AddToShelfCommand.h"
#include "commands/RemoveFromShelfCommand.h"
#include "commands/ShowShelfCommand.h"
#include "commands/ShowInboxCommand.h"
#include "commands/ReadMsgCommand.h"
#include "commands/DeleteMsgCommand.h"
#include "commands/FriendsCommand.h"
#include "commands/AddBirthdayCommand.h"
#include "commands/ProfileCommand.h"
#include "commands/AcceptOfferCommand.h"
#include "commands/LeaveJobCommand.h"
#include "commands/FollowersCommand.h"
#include "commands/PublishCommand.h"
#include "commands/AddSynopsisCommand.h"
#include "commands/OfferCommand.h"

#include "repositories/UserRepository.h"
#include "repositories/BookRepository.h"
#include "services/AuthService.h"
#include "services/UserService.h"
#include "services/BookService.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> parseInput(const std::string& input);
void executeCommand(const std::string& input, AuthService& authService, UserService& userService, BookService& bookService, BookRepository& bookRepo);

void run()
{
    UserRepository userRepo("users.dat");
    BookRepository bookRepo("books.dat");

    AuthService authService(userRepo);
    UserService userService(userRepo, bookRepo);
    BookService bookService(userRepo, bookRepo);

    std::string input;

    while (true) {
        std::getline(std::cin, input);

        if (input.empty()) { 
            continue; 
        }

        if (input == "exit") {
            break;
        }

        executeCommand(input, authService, userService, bookService, bookRepo);
    }
}

void executeCommand(const std::string& input,
    AuthService& authService,
    UserService& userService,
    BookService& bookService,
    BookRepository& bookRepo)
{
    auto tokens = parseInput(input);
    if (tokens.empty()) {
        return;
    }

    const std::string& command = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    if (command == "help") {
        HelpCommand(authService).execute(args);
    }
    else if (command == "register") {
        RegisterCommand(authService).execute(args);
    }
    else if (command == "login") {
        LoginCommand(authService).execute(args);
    }
    else if (command == "logout") {
        LogoutCommand(authService).execute(args);
    }
    else if (command == "search") {
        SearchCommand(authService, userService).execute(args);
    }
    else if (command == "follow") {
        FollowCommand(authService, userService).execute(args);
    }
    else if (command == "add-book") {
        AddBookCommand(authService, bookService).execute(args);
    }
    else if (command == "delete-book") {
        DeleteBookCommand(authService, bookService).execute(args);
    }
    else if (command == "create-shelf") {
        CreateShelfCommand(authService, bookService).execute(args);
    }
    else if (command == "delete-shelf") {
        DeleteShelfCommand(authService, bookService).execute(args);
    }
    else if (command == "add-to-shelf") {
        AddToShelfCommand(authService, bookService).execute(args);
    }
    else if (command == "remove-from-shelf") {
        RemoveFromShelfCommand(authService, bookService).execute(args);
    }
    else if (command == "show-shelf") {
        ShowShelfCommand(authService, bookService, bookRepo).execute(args);
    }
    else if (command == "show-inbox") {
        ShowInboxCommand(authService, userService).execute(args);
    }
    else if (command == "read-msg") {
        ReadMsgCommand(authService, userService).execute(args);
    }
    else if (command == "delete-msg") {
        DeleteMsgCommand(authService, userService).execute(args);
    }
    else if (command == "friends") {
        FriendsCommand(authService, userService).execute(args);
    }
    else if (command == "add-birthday") {
        AddBirthdayCommand(authService, userService).execute(args);
    }
    else if (command == "profile") {
        ProfileCommand(authService, userService).execute(args);
    }
    else if (command == "accept-offer") {
        AcceptOfferCommand(authService, userService).execute(args);
    }
    else if (command == "leave") {
        LeaveJobCommand(authService, userService).execute(args);
    }
    else if (command == "followers") {
        FollowersCommand(authService, userService).execute(args);
    }
    else if (command == "publish") {
        PublishCommand(authService, bookService).execute(args);
    }
    else if (command == "add-synopsis") {
        AddSynopsisCommand(authService, bookService).execute(args);
    }
    else if (command == "offer") {
        OfferCommand(authService, userService).execute(args);
    }
    else {
        std::cout << "Unknown command!. Type help to see all commands!\n";
    }
}

std::vector<std::string> parseInput(const std::string& input)
{
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

int main()
{
    run();
    return 0;
}