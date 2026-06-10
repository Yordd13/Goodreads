#pragma once
#include <string>
#include <vector>
#include "users/User.h"

class Publisher : public User
{
private:
    std::vector<std::string> authorNames;
    std::vector<std::string> publishedTitles;

protected:
    void serializeExtra(std::ostream& out)  const override;
    void deserializeExtra(std::istream& in) override;

public:
    Publisher() = default;
    Publisher(const std::string& username, const std::string& password, const Date& registrationDate);

    const std::vector<std::string>& getAuthorNames() const;
    const std::vector<std::string>& getPublishedTitles() const;

    void addAuthor(const std::string& authorName);
    void addPublishedTitle(const std::string& title);
    bool hasAuthor(const std::string& authorName) const;
    void removeAuthor(const std::string& authorName);

    std::string getType() const override;
    void showProfile() const override;
};