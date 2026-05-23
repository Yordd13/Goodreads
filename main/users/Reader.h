#pragma once
#include <string>
#include <vector>
#include "users/User.h"
#include "models/Shelf.h"
#include "models/Message.h"
#include "models/Date.h"
#include "models/PersonalLibrary.h"

class Reader : public User
{
    friend class Shelf;
    friend class Message;

private:
    std::vector<PersonalLibrary> readingEntries;
    std::vector<Shelf> shelves;
    std::vector<std::string> favoriteBookTitles;
    std::vector<Message> inbox;
    bool hasBirthdaySet;
    Date birthday;

protected:
    void serializeExtra(std::ostream& out)  const override;
    void deserializeExtra(std::istream& in) override;

public:
    Reader() = default;
    Reader(const std::string& username,
        const std::string& password,
        const Date& registrationDate);

    const std::vector<PersonalLibrary>& getPersonalLibrary() const;
    const std::vector<Shelf>& getShelves() const;
    const std::vector<std::string>& getFavorites() const;
    const std::vector<Message>& getInbox() const;
    bool hasBirthday() const;
    Date getBirthday() const;

    bool addBook(const std::string& title, ReadStatus status, int rating = -1);

    bool deleteBook(const std::string& title);
    bool hasBook(const std::string& title) const;
    PersonalLibrary* findPersonalLibrary(const std::string& title);
    const PersonalLibrary* findPersonalLibrary(const std::string& title) const;

    bool createShelf(const std::string& name);
    bool  deleteShelf(const std::string& name);
    bool  addToShelf(const std::string& bookTitle, const std::string& shelfName);
    bool removeFromShelf(const std::string& bookTitle, const std::string& shelfName);
    Shelf* findShelf(const std::string& name);
    const Shelf* findShelf(const std::string& name) const;

    void receiveMessage(const Message& msg);
    Message* getMessageAt(int index);
    bool deleteMessage(int index);

    void setBirthday(const Date& date);
    void clearBirthday();
    void addFavorite(const std::string& bookTitle);
    void removeFavorite(const std::string& bookTitle);

    std::string getType() const override;
    void showProfile() const override;
};