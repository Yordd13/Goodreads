#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"

class HelpCommand : public Command
{
private:

    AuthService& authService;

    void showGlobalHelp() const;
    void showReaderHelp() const;
    void showAuthorHelp() const;
    void showPublisherHelp() const;

public:

    HelpCommand(AuthService& authService);
    void execute(const std::vector<std::string>& data) override;

};

