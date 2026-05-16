#include "models/Shelf.h"

Shelf::Shelf(const std::string& name, const Date& creationDate)
{
	//TODO
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
	return bookCount;
}

const std::vector<std::string>& Shelf::getBooks() const
{
	return books;
}

bool Shelf::addBook(const std::shared_ptr<Book>& book)
{
	//TODO: check if the book already exists in the shelf, if it does, return false
	return false;
}

bool Shelf::removeBook(const std::string& title)
{
	//TODO: find the book by title, if it exists, remove it from the shelf and return true, otherwise
	return false;
}

bool Shelf::hasBook(const std::string& title) const
{
	for (int i = 0; i < bookCount; i++) {
		if(books[i] == title) {
			return true;
		}
	}
	return false;
}

void Shelf::toSerial(std::ostream& out) const {
	//TODO
}
Shelf Shelf::fromSerial(std::istream& in) {
	//TODO
	return Shelf();
}