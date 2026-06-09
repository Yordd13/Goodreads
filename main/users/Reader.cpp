#include "users/Reader.h"
#include <iostream>

Reader::Reader(const std::string& username, const std::string& password, const Date& registrationDate) : User(username, password, registrationDate), hasBirthdaySet(false), birthday()
{}

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
	return hasBirthdaySet;
}

Date Reader::getBirthday() const
{
    return birthday;
}

bool Reader::addBook(const std::string& title, ReadStatus status, int rating)
{
	if (hasBook(title)) {
		return false;
	}

	personalLibrary.push_back(PersonalLibrary(title, status, rating));
	return true;
}

bool Reader::deleteBook(const std::string& title)
{
	auto it = std::find_if(personalLibrary.begin(), personalLibrary.end(),
		[&title](const PersonalLibrary& e)
		{
			return e.title == title;
		});

	if (it == personalLibrary.end()) {
		return false;
	}

	personalLibrary.erase(it);

	for (auto& shelf : shelves) {
		shelf.removeBook(title);
	}

	removeFavorite(title);

	return true;
}

bool Reader::hasBook(const std::string& title) const
{
	return findPersonalLibrary(title) != nullptr;
}

PersonalLibrary* Reader::findPersonalLibrary(const std::string& title)
{
	auto it = std::find_if(personalLibrary.begin(), personalLibrary.end(),
		[&title](const PersonalLibrary& e)
		{
			return e.title == title;
		});

	return it != personalLibrary.end() ? &(*it) : nullptr;
}

const PersonalLibrary* Reader::findPersonalLibrary(const std::string& title) const
{
	auto it = std::find_if(personalLibrary.begin(), personalLibrary.end(),
		[&title](const PersonalLibrary& e)
		{
			return e.title == title;
		});

	return it != personalLibrary.end() ? &(*it) : nullptr;
}

bool Reader::createShelf(const std::string& name)
{
	if (findShelf(name) != nullptr) {
		return false;
	}

	//TODO: use current date
	shelves.push_back(Shelf(name, Date(1, 1, 2000)));
	return true;
}

bool Reader::deleteShelf(const std::string& name)
{
	auto it = std::find_if(shelves.begin(), shelves.end(),
		[&name](const Shelf& s)
		{
			return s.getName() == name;
		});

	if (it == shelves.end()) {
		return false;
	}

	shelves.erase(it);
	return true;
}

bool Reader::addToShelf(const std::string& bookTitle, const std::string& shelfName)
{
	if (!hasBook(bookTitle)) {
		return false;
	}

	Shelf* shelf = findShelf(shelfName);
	if (!shelf){
		return false;
	}

	return shelf->addBook(bookTitle);
}

bool Reader::removeFromShelf(const std::string& bookTitle, const std::string& shelfName)
{
	Shelf* shelf = findShelf(shelfName);
	if (!shelf) {
		return false;
	}

	return shelf->removeBook(bookTitle);
}

Shelf* Reader::findShelf(const std::string& name)
{
	auto it = std::find_if(shelves.begin(), shelves.end(),
		[&name](const Shelf& s)
		{
			return s.getName() == name;
		});

	return it != shelves.end() ? &(*it) : nullptr;
}

const Shelf* Reader::findShelf(const std::string& name) const
{
	auto it = std::find_if(shelves.begin(), shelves.end(),
		[&name](const Shelf& s)
		{
			return s.getName() == name;
		});

	return it != shelves.end() ? &(*it) : nullptr;
}

void Reader::receiveMessage(const Message& msg)
{
	inbox.push_back(msg);
}

Message* Reader::getMessageAt(int index)
{
	if (index < 0 || index >= inbox.size()) {
		return nullptr;
	}
	return &inbox[index];
}

bool Reader::deleteMessage(int index)
{
	if (index < 0 || index >= inbox.size()) {
		return false;
	}

	if (!inbox[index].isMessageRead()) {
		return false;
	}

	inbox.erase(inbox.begin() + index);
	return true;
}

void Reader::setBirthday(const Date& date)
{
	birthday = date;
	hasBirthdaySet = true;
}

void Reader::clearBirthday()
{
	hasBirthdaySet = false;
}

void Reader::addFavorite(const std::string& bookTitle)
{
	if (std::find(favoriteBookTitles.begin(), favoriteBookTitles.end(), bookTitle) == favoriteBookTitles.end()){
		favoriteBookTitles.push_back(bookTitle);
	}
}

void Reader::removeFavorite(const std::string& bookTitle)
{
	auto it = std::find(favoriteBookTitles.begin(), favoriteBookTitles.end(), bookTitle);

	if (it != favoriteBookTitles.end()) {
		favoriteBookTitles.erase(it);
	}
}

std::string Reader::getType() const
{
	return "Reader";
}

void Reader::showProfile() const
{
	std::cout << "    Reader Profile    \n";
	std::cout << "Username:    " << getUsername() << "\n";
	std::cout << "Registered:  " << getRegistrationDate().toString() << "\n";

	if (hasBirthdaySet) {
		std::cout << "Birthday:    " << birthday.toString() << "\n";
	}

	std::cout << "Followers:   " << getFollowers().size() << "\n";

	std::cout << "Library (" << personalLibrary.size() << " books):\n";
	for (const auto& e : personalLibrary) {
		std::cout << "  - " << e.title << "\n";
	}

	std::cout << "Shelves (" << shelves.size() << "):\n";
	for (const auto& s : shelves) {
		std::cout << "  - " << s.getName() << " (" << s.getBookCount() << " books)\n";
	}

	if (!favoriteBookTitles.empty()){
		std::cout << "Favorites:\n";
		for (const auto& f : favoriteBookTitles) {
			std::cout << "  - " << f << "\n";
		}
	}
}

void Reader::serializeExtra(std::ostream& out) const
{
	out << personalLibrary.size() << '\n';
	for (const auto& e : personalLibrary) {
		e.serialize(out);
	}

	out << shelves.size() << '\n';
	for (const auto& s : shelves) {
		s.toSerial(out);
	}

	out << favoriteBookTitles.size() << '\n';
	for (const auto& f : favoriteBookTitles) {
		out << f << '\n';
	}

	out << (hasBirthdaySet ? 1 : 0) << '\n';
	if (hasBirthdaySet) {
		out << birthday.toSerial() << '\n';
	}

	out << inbox.size() << '\n';
	for (const auto& msg : inbox) {
		msg.toSerial(out);
	}
}

void Reader::deserializeExtra(std::istream& in)
{
	size_t libCount;
	in >> libCount;
	in.ignore();

	personalLibrary.clear();
	for (size_t i = 0; i < libCount; i++) {
		personalLibrary.push_back(PersonalLibrary::deserialize(in));
	}

	size_t shelfCount;
	in >> shelfCount;
	in.ignore();

	shelves.clear();
	for (size_t i = 0; i < shelfCount; i++) {
		shelves.push_back(Shelf::fromSerial(in));
	}

	size_t favCount;
	in >> favCount;
	in.ignore();

	favoriteBookTitles.clear();
	for (size_t i = 0; i < favCount; i++){
		std::string title;
		std::getline(in, title);
		favoriteBookTitles.push_back(title);
	}

	int birthdayFlag;
	in >> birthdayFlag;
	in.ignore();

	hasBirthdaySet = (birthdayFlag == 1);
	if (hasBirthdaySet){
		int serialDate;
		in >> serialDate;
		in.ignore();
		birthday = Date::fromSerial(serialDate);
	}

	size_t inboxCount;
	in >> inboxCount;
	in.ignore();

	inbox.clear();
	for (size_t i = 0; i < inboxCount; i++) {
		inbox.push_back(Message::fromSerial(in));
	}
}