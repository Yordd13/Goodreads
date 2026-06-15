#pragma once
#include "commands/Command.h"
#include "services/AuthService.h"
#include "services/UserService.h"

class AddBirthdayCommand : public Command
{
private:

    AuthService& authService;
    UserService& userService;

public:

    AddBirthdayCommand(AuthService& authService, UserService& userService);
    void execute(const std::vector<std::string>& data) override;

};