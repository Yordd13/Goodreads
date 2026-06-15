#include "users/Publisher.h"
#include <iostream>

Publisher::Publisher(const std::string& username, const std::string& password, const Date& registrationDate) : User(username, password, registrationDate)
{}

const std::vector<std::string>& Publisher::getAuthorNames() const
{
	return authorNames;
}

const std::vector<std::string>& Publisher::getPublishedTitles() const
{
    return publishedTitles;
}

void Publisher::addAuthor(const std::string& authorName)
{
    if (!hasAuthor(authorName)) {
        authorNames.push_back(authorName);
    }
}

void Publisher::addPublishedTitle(const std::string& title)
{
    if (std::find(publishedTitles.begin(), publishedTitles.end(), title) == publishedTitles.end()) {
        publishedTitles.push_back(title);
    }
}

bool Publisher::hasAuthor(const std::string& authorName) const
{
    return std::find(authorNames.begin(), authorNames.end(), authorName) != authorNames.end();
}

void Publisher::removeAuthor(const std::string& authorName)
{
    auto author = std::find(authorNames.begin(), authorNames.end(), authorName);
    if (author != authorNames.end()) {
        authorNames.erase(author);
    }
}

std::string Publisher::getType() const
{
    return "Publisher";
}

void Publisher::showProfile() const
{
    std::cout << "    Publisher Profile    \n";
    std::cout << "Username:   " << getUsername() << "\n";
    std::cout << "Registered: " << getRegistrationDate().toString() << "\n";
    std::cout << "Followers:  " << getFollowers().size() << "\n";

    std::cout << "Authors worked with (" << authorNames.size() << "):\n";
    for (const auto& a : authorNames) {
        std::cout << "  - " << a << "\n";
    }

    std::cout << "Published books (" << publishedTitles.size() << "):\n";

    for (const auto& t : publishedTitles) {
        std::cout << "  - " << t << "\n";
    }
}

void Publisher::serializeExtra(std::ostream& out) const
{
    out << authorNames.size() << '\n';
    for (const auto& author : authorNames) {
        out << author << '\n';
    }

    out << publishedTitles.size() << '\n';
    for (const auto& title : publishedTitles) {
        out << title << '\n';
    }
}

void Publisher::deserializeExtra(std::istream& in)
{
    size_t authorCount;
    in >> authorCount;
    in.ignore();

    authorNames.clear();
    for (size_t i = 0; i < authorCount; i++){
        std::string name;
        std::getline(in, name);
        authorNames.push_back(name);
    }

    size_t titleCount;
    in >> titleCount;
    in.ignore();

    publishedTitles.clear();
    for (size_t i = 0; i < titleCount; i++){
        std::string title;
        std::getline(in, title);
        publishedTitles.push_back(title);
    }
}
