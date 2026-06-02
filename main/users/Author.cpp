#include "users/Author.h"
#include <iostream>
#include <algorithm>

Author::Author(const std::string& username, const std::string& password, const Date& registrationDate) : Reader(username, password, registrationDate)
{}

const std::vector<std::string>& Author::getPublishedTitles() const
{
	return publishedTitles;
}

const std::vector<std::string>& Author::getPublisherNames() const
{
	return publisherNames;
}

void Author::addPublishedTitle(const std::string& title)
{
	bool alreadyExists = std::find(publishedTitles.begin(), publishedTitles.end(), title) != publishedTitles.end();
    if (!alreadyExists){
        publishedTitles.push_back(title);
    }
}

void Author::addPublisher(const std::string& publisherName)
{
    if(!hasPublisher(publisherName)){
        publisherNames.push_back(publisherName);
	}
}

bool Author::removePublisher(const std::string& publisherName)
{
    auto it = std::find(publisherNames.begin(), publisherNames.end(), publisherName);
    if (it == publisherNames.end()) {
        return false;
    }

    publisherNames.erase(it);
    return true;
}

bool Author::hasPublisher(const std::string& publisherName) const
{
    return std::find(publisherNames.begin(), publisherNames.end(), publisherName) != publisherNames.end();
}

std::string Author::getType() const
{
    return "Author";
}

void Author::showProfile() const
{
    std::cout << "    Author Profile    \n";
    std::cout << "Username: " << getUsername() << "\n";
    std::cout << "Registered: " << getRegistrationDate().toString() << "\n";

    if (hasBirthday()){
        std::cout << "Birthday: " << getBirthday().toString() << "\n";
    }

    std::cout << "Followers: " << getFollowers().size() << "\n";

    std::cout << "Published books (" << publishedTitles.size() << "):\n";

    for (const auto& title : publishedTitles){
        std::cout << "  - " << title << "\n";
    }

    std::cout << "Publishers worked with (" << publisherNames.size() << "):\n";

    for (const auto& publisherName : publisherNames){
        std::cout << "  - " << publisherName << "\n";
    }

    std::cout << "Shelves (" << getShelves().size() << "):\n";

    for (const auto& shelf : getShelves()){
        std::cout << "  - " << shelf.getName() << " (" << shelf.getBookCount() << " books)\n";
    }

    if (!getFavorites().empty()){
        std::cout << "Favorite books:\n";
        for (const auto& favorite : getFavorites()){
            std::cout << "  - " << favorite << "\n";
        }
    }
}


void Author::serializeExtra(std::ostream& out) const
{
    Reader::serializeExtra(out);

    out << publishedTitles.size() << '\n';

    for (const auto& title : publishedTitles) {
        out << title << '\n';
    }

    out << publisherNames.size() << '\n';

    for (const auto& publisherName : publisherNames) {
        out << publisherName << '\n';
    }
}

void Author::deserializeExtra(std::istream& in)
{
    Reader::deserializeExtra(in);

    size_t titleCount;
    in >> titleCount;
    in.ignore();

    publishedTitles.clear();
    for (size_t i = 0; i < titleCount; i++){
        std::string title;
        std::getline(in, title);
        publishedTitles.push_back(title);
    }

    size_t publisherCount;
    in >> publisherCount;
    in.ignore();

    publisherNames.clear();
    for (size_t i = 0; i < publisherCount; i++){
        std::string name;
        std::getline(in, name);
        publisherNames.push_back(name);
    }
}