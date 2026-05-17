#include "models/Shelf.h"
#include "validator/ShelfValidation.h"
#include <stdexcept>
#include <iostream>

void Shelf::setName(const std::string& name)
{
	if(shelfValidation::validateName(name)) {
		this->name = name;
	}
	else {
		throw std::invalid_argument("Invalid shelf name");
	}
}

Shelf::Shelf(const std::string& name, const Date& creationDate) : creationDate(creationDate)
{
	setName(name);
}

std::string Shelf::getName() const
{
	return name;
}

const Date& Shelf::getCreationDate() const
{
	return creationDate;
}

size_t Shelf::getBookCount() const
{
	return books.size();
}

const std::vector<std::string>& Shelf::getBooks() const
{
	return books;
}

bool Shelf::addBook(const std::string& title)
{
	if (hasBook(title)) {
		return false;
	}

	books.push_back(title);
	return true;
}

bool Shelf::removeBook(const std::string& title)
{
	auto it = std::find(books.begin(), books.end(), title);
	if (it == books.end()) {
		return false;
	}

	books.erase(it);
	return true;
}

bool Shelf::hasBook(const std::string& title) const
{
	return std::find(books.begin(), books.end(), title) != books.end();
}

void Shelf::toSerial(std::ostream& out) const {
	//   name
	//   DD MM YYYY
	//   bookCount
	//   title1
	//   title2
	//   ...

	out << name << '\n';
	out << creationDate.toSerial() << '\n';

	out << books.size() << '\n';

	for (const auto& title : books) {
		out << title << '\n';
	}
}

Shelf Shelf::fromSerial(std::istream& in) {

	std::string name;
	std::getline(in, name);

	int serialDate;
	in >> serialDate;
	Date creationDate = Date::fromSerial(serialDate);
	in.ignore();    

	size_t count;
	in >> count;
	in.ignore();    

	Shelf shelf(name, creationDate);

	for (size_t i = 0; i < count; i++)
	{
		std::string title;
		std::getline(in, title);
		shelf.books.push_back(title);
	}

	return shelf;
}