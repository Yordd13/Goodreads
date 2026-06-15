#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/UserService.h"

class ReadMsgCommand : public Command
{
private:

    AuthService& authService;
    UserService& userService;

public:

    ReadMsgCommand(AuthService& authService, UserService& userService);
    void execute(const std::vector<std::string>& data) override;

};