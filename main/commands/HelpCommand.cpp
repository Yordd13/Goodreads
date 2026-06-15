#include "commands/HelpCommand.h"
#include <iostream>

HelpCommand::HelpCommand(AuthService& authService) : authService(authService)
{}

void HelpCommand::execute(const std::vector<std::string>& data)
{
    showGlobalHelp();

    if (!authService.isLoggedIn()){
        return;
    }

    std::string type = authService.getCurrentUserType();

    if (type == "Reader") {
        showReaderHelp();
    }
    else if (type == "Author") {
        showReaderHelp();
        showAuthorHelp();
    }
    else if (type == "Publisher") {
        showPublisherHelp();
    }
}

void HelpCommand::showGlobalHelp() const
{
    std::cout << "\n    General Commands    \n";
    std::cout << "  help\n";
    std::cout << "  register <username> <password> <type>\n";
    std::cout << "  login <username> <password>\n";
    std::cout << "  logout\n";
    std::cout << "  search <name>\n";
    std::cout << "  exit\n";
}

void HelpCommand::showReaderHelp() const
{
    std::cout << "\n    Reader Commands    \n";
    std::cout << "  follow <username>\n";
    std::cout << "  add-book <bookName> <status> [rating]\n";
    std::cout << "  delete-book <bookName>\n";
    std::cout << "  create-shelf <name>\n";
    std::cout << "  delete-shelf <name>\n";
    std::cout << "  add-to-shelf <bookName> <shelfName>\n";
    std::cout << "  remove-from-shelf <bookName> <shelfName>\n";
    std::cout << "  show-shelf [reader] <name>\n";
    std::cout << "  show-inbox [filter]\n";
    std::cout << "  read-msg <index>\n";
    std::cout << "  delete-msg <index>\n";
    std::cout << "  friends [reader]\n";
    std::cout << "  add-birthday [date]\n";
    std::cout << "  profile [reader]\n";
}

void HelpCommand::showAuthorHelp() const
{
    std::cout << "\n    Author Commands    \n";
    std::cout << "  show-inbox [filter]\n";
    std::cout << "  accept-offer <index>\n";
    std::cout << "  leave <publisher>\n";
    std::cout << "  followers\n";
}

void HelpCommand::showPublisherHelp() const
{
    std::cout << "\n    Publisher Commands    \n";
    std::cout << "  publish <bookTitle> <authorName> <releaseDate> <pageCount> <genres...>\n";
    std::cout << "  add-synopsis <bookTitle> <synopsis>\n";
    std::cout << "  offer <author>\n";
    std::cout << "  show-inbox\n";
    std::cout << "  read-msg <index>\n";
    std::cout << "  delete-msg <index>\n";
}