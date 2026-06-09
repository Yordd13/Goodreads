#pragma once
#include <string>
#include <vector>
#include "models/Date.h"

class Shelf{

	friend class Reader;

private:

	std::string name;
	std::vector<std::string> books;
	Date creationDate;

	void setName(const std::string& name);

public:

	Shelf() = default;
	Shelf(const std::string& name, const Date& creationDate);

	std::string getName() const;
	const Date& getCreationDate() const;
	size_t getBookCount() const;

	const std::vector<std::string>& getBooks() const;


	bool addBook(const std::string& title);
	bool removeBook(const std::string& title);
	bool hasBook(const std::string& title) const;

private:

	void toSerial(std::ostream& out) const;
	static Shelf fromSerial(std::istream& in);

};

