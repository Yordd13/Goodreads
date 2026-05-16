#pragma once
#include <string>
#include <vector>
#include <memory>
#include "models/Book.h"
#include "models/Date.h"

class Shelf
{
private:

	std::string name;
	std::vector<std::string> books;
	Date creationDate;
	size_t bookCount;

public:

	Shelf() = default;
	Shelf(const std::string& name, const Date& creationDate);

	std::string getName() const;
	const Date& getCreationDate() const;
	size_t getBookCount() const;

	const std::vector<std::string>& getBooks() const;


	bool addBook(const std::shared_ptr<Book>& book);
	bool removeBook(const std::string& title);
	bool hasBook(const std::string& title) const;

private:

	void toSerial(std::ostream& out) const;
	static Shelf fromSerial(std::istream& in);

};

