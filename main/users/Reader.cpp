#include "users/Reader.h"

void Reader::serializeExtra(std::ostream& out) const
{
}

void Reader::deserializeExtra(std::istream& in)
{
}

Reader::Reader(const std::string& username, const std::string& password, const Date& registrationDate)
{
}

const std::vector<PersonalLibrary>& Reader::getPersonalLibrary() const
{
	return personalLibrary;
}

const std::vector<Shelf>& Reader::getShelves() const
{
    return shelves;
}

const std::vector<std::string>& Reader::getFavorites() const
{
    return favoriteBookTitles;
}

const std::vector<Message>& Reader::getInbox() const
{
    return inbox;
}

bool Reader::hasBirthday() const
{
    return false;
}

Date Reader::getBirthday() const
{
    return Date();
}

bool Reader::addBook(const std::string& title, ReadStatus status, int rating)
{
    return false;
}

bool Reader::deleteBook(const std::string& title)
{
    return false;
}

PersonalLibrary* Reader::findPersonalLibrary(const std::string& title)
{
	return nullptr;
}

const PersonalLibrary* Reader::findPersonalLibrary(const std::string& title) const
{
	return nullptr;
}

bool Reader::createShelf(const std::string& name)
{
	return false;
}

bool Reader::deleteShelf(const std::string& name)
{
	return false;
}

bool Reader::addToShelf(const std::string& bookTitle, const std::string& shelfName)
{
	return false;
}

bool Reader::removeFromShelf(const std::string& bookTitle, const std::string& shelfName)
{
	return false;
}

Shelf* Reader::findShelf(const std::string& name)
{
	return nullptr;
}

const Shelf* Reader::findShelf(const std::string& name) const
{
	return nullptr;
}

void Reader::receiveMessage(const Message& msg)
{
}

Message* Reader::getMessageAt(int index)
{
	return nullptr;
}

bool Reader::deleteMessage(int index)
{
	return false;
}

void Reader::setBirthday(const Date& date)
{
}

void Reader::clearBirthday()
{
}

void Reader::addFavorite(const std::string& bookTitle)
{
}

void Reader::removeFavorite(const std::string& bookTitle)
{
}

std::string Reader::getType() const
{
	return std::string();
}

void Reader::showProfile() const
{
}