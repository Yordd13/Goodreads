#pragma once
#include <string>
#include <vector>
#include <optional>
#include "repositories/UserRepository.h"
#include "repositories/BookRepository.h"
#include "users/Reader.h"
#include "users/Author.h"
#include "users/Publisher.h"
#include "models/PersonalLibrary.h"
#include "models/Shelf.h"
#include "models/Date.h"

class BookService
{
private:

    UserRepository& userRepo;
    BookRepository& bookRepo;

    void notifyFollowers(const std::string& authorUsername, const std::string& publisherUsername, const std::string& bookTitle);

public:

    BookService(UserRepository& userRepo, BookRepository& bookRepo);

    void addBook(Reader* currentReader, const std::string& title, ReadStatus status, int rating = -1);

    void deleteBook(Reader* currentReader, const std::string& title);

    void createShelf(Reader* currentReader, const std::string& shelfName);

    void deleteShelf(Reader* currentReader, const std::string& shelfName);

    void addToShelf(Reader* currentReader, const std::string& bookTitle, const std::string& shelfName);

    void removeFromShelf(Reader* currentReader, const std::string& bookTitle, const std::string& shelfName);

    const Shelf* getShelf(Reader* currentReader, const std::string& shelfName, std::optional<std::string> targetUsername = std::nullopt) const;

    void publish(Publisher* currentPublisher, const std::string& bookTitle, const std::string& authorUsername, const Date& releaseDate, size_t pageCount, const std::vector<std::string>& genres);

    void addSynopsis(Publisher* currentPublisher, const std::string& bookTitle, const std::string& synopsis);

};