#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"

class LoginCommand : public Command
{
private:

    AuthService& authService;

public:

    LoginCommand(AuthService& authService);
    void execute(const std::vector<std::string>& data) override;

};
