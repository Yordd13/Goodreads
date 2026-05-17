#include "validator/MessageValidation.h"

bool messageValidation::validateName(const std::string& name)
{
	return !name.empty();
}

bool messageValidation::validateContent(const std::string& content)
{
    return !content.empty();
}
