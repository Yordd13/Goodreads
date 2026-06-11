#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"

class RegisterCommand : public Command
{
private:

    AuthService& authService;

public:

    RegisterCommand(AuthService& authService);
    void execute(const std::vector<std::string>& data) override;

};

