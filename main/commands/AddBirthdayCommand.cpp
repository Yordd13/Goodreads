#include "commands/AddBirthdayCommand.h"
#include "models/Date.h"
#include <iostream>

AddBirthdayCommand::AddBirthdayCommand(AuthService& authService, UserService& userService) : authService(authService), userService(userService)
{}

void AddBirthdayCommand::execute(const std::vector<std::string>& data)
{
    Reader* reader = authService.getCurrentReader();
    if (reader == nullptr) {
        std::cout << "Error: only readers and authors can set a birthday!\n";
        return;
    }

    try {
        if (data.size() < 1 || data[0].empty()) {
            userService.clearBirthday(reader);
            std::cout << "Birthday was removed!\n";
        }
        else {
            Date birthday = Date::fromString(data[0]);
            userService.addBirthday(reader, birthday);
            std::cout << "Birthday set successfully!\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}