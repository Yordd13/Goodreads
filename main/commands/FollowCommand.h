#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/UserService.h"

class FollowCommand : public Command
{
private:

    AuthService& authService;
    UserService& userService;

public:

    FollowCommand(AuthService& authService, UserService& userService);
    void execute(const std::vector<std::string>& data) override;

};
