#pragma once
#include <string>
#include <vector>
#include "repositories/Repository.hpp"
#include "models/Book.h"

class BookRepository : public Repository<Book>
{
private:
    std::string filePath;

    void loadFromFile();
    void saveToFile() const;

    static int editDistance(const std::string& a, const std::string& b);

public:
    BookRepository(const std::string& filePath);
    ~BookRepository();

    const Book* findByTitle(const std::string& title) const;
    Book* getMutable(const std::string& title);
    bool titleExists(const std::string& title) const;

    std::vector<const Book*> findByAuthor(const std::string& authorName) const;
    std::vector<const Book*> findByPublisher(const std::string& publisherName) const;
    std::vector<const Book*> findByGenre(const std::string& genre) const;

    std::vector<const Book*> fuzzySearchByTitle(const std::string& query) const;
};

