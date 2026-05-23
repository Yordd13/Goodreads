#pragma once
#include <string>
#include <vector>
#include "users/Reader.h"

class Author : public Reader
{
private:
    std::vector<std::string> publishedTitles;
    std::vector<std::string> publisherNames;

protected:
    void serializeExtra(std::ostream& out)  const override;
    void deserializeExtra(std::istream& in) override;

public:
    Author() = default;
    Author(const std::string& username, const std::string& password, const Date& registrationDate);

    const std::vector<std::string>& getPublishedTitles() const;
    const std::vector<std::string>& getPublisherNames() const;

    void addPublishedTitle(const std::string& title);
    void addPublisher(const std::string& publisherName);
    bool removePublisher(const std::string& publisherName);
    bool hasPublisher(const std::string& publisherName) const;

    std::string getType() const override;
    void showProfile() const override;
};