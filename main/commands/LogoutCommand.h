#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"

class LogoutCommand : public Command
{
private:

    AuthService& authService;

public:

    LogoutCommand(AuthService& authService);
    void execute(const std::vector<std::string>& data) override;

};
