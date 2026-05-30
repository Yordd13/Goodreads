#include "validator/UserValidation.h"
#include "constants/UserResources.h"
#include <cctype>

bool userValidation::isValidUsername(const std::string& username)
{
	return username.size() >= resources::user::MIN_NAME_LENGTH
		&& username.size() <= resources::user::MAX_NAME_LENGTH;
}

bool userValidation::isValidPassword(const std::string& password)
{
    if (password.size() < resources::user::MIN_PASSWORD_LENGTH || password.size() > resources::user::MAX_PASSWORD_LENGTH) {
        return false;
    }

    bool hasLower = false;
    bool hasUpper = false;
    bool hasNonLetter = false;

    for (char c : password)
    {
        if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        if (!std::isalpha(static_cast<unsigned char>(c))) hasNonLetter = true;
    }

    return hasLower && hasUpper && hasNonLetter;
}
