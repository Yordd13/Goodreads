#include "services/BookService.h"
#include "users/Author.h"
#include <stdexcept>
#include <algorithm>

BookService::BookService(UserRepository& userRepo, BookRepository& bookRepo) : userRepo(userRepo), bookRepo(bookRepo)
{}

void BookService::addBook(Reader* currentReader, const std::string& title, ReadStatus status, int rating)
{
    if(!bookRepo.titleExists(title)){
		throw std::runtime_error("Book with this title does not exist!");
	}

	if(currentReader->hasBook(title)){
		throw std::invalid_argument("Book already exists in your library!");
	}

	if (rating != -1) {
		Book* book = bookRepo.getMutable(title);
		book->addRating(rating);
	}

	currentReader->addBook(title, status, rating);
}

void BookService::deleteBook(Reader* currentReader, const std::string& title)
{
	if(!currentReader->hasBook(title)){
		throw std::runtime_error("Book is not in your library!");
	}

	currentReader->deleteBook(title);
}

void BookService::createShelf(Reader* currentReader, const std::string& shelfName)
{
	if(currentReader->findShelf(shelfName) != nullptr){
		throw std::invalid_argument("Shelf with this name already exists!");
	}

	currentReader->createShelf(shelfName);
}

void BookService::deleteShelf(Reader* currentReader, const std::string& shelfName)
{
	if(currentReader->findShelf(shelfName) == nullptr){
		throw std::runtime_error("Shelf with this name does not exist!");
	}

	currentReader->deleteShelf(shelfName);
}

void BookService::addToShelf(Reader* currentReader, const std::string& bookTitle, const std::string& shelfName)
{
	if(currentReader->findShelf(shelfName) == nullptr){
		throw std::runtime_error("Shelf with this name does not exist!");
	}

	if(!currentReader->hasBook(bookTitle)){
		throw std::runtime_error("Book is not in your library!");
	}

	const Shelf* shelf = currentReader->findShelf(shelfName);
	if(shelf->hasBook(bookTitle)){
		throw std::invalid_argument("Book is already in this shelf!");
	}

	currentReader->addToShelf(bookTitle, shelfName);
}

void BookService::removeFromShelf(Reader* currentReader, const std::string& bookTitle, const std::string& shelfName)
{
	if(currentReader->findShelf(shelfName) == nullptr){
		throw std::runtime_error("Shelf with this name does not exist!");
	}

	const Shelf* shelf = currentReader->findShelf(shelfName);
	if(!shelf->hasBook(bookTitle)){
		throw std::runtime_error("Book is not in this shelf!");
	}

	currentReader->removeFromShelf(bookTitle, shelfName);
}

const Shelf* BookService::getShelf(Reader* currentReader, const std::string& shelfName, std::optional<std::string> targetUsername) const
{
	if (!targetUsername.has_value()) {
		const Shelf* shelf = currentReader->findShelf(shelfName);
		if (!shelf) {
			throw std::runtime_error("Shelf " + shelfName + " not found!");
		}
		return shelf;
	}

	const std::string& target = targetUsername.value();

	auto friends = userRepo.getFriendsOf(currentReader->getUsername());
	bool areFriends = std::any_of(friends.begin(), friends.end(),
		[&target](const User* u) { return u->getUsername() == target; 
	});

	if (!areFriends) {
		throw std::runtime_error("You are not friends with " + target + "!");
	}

	User* targetUser = userRepo.getMutable(target);
	if (targetUser == nullptr) {
		throw std::runtime_error("User not found!");
	}

	Reader* targetReader = dynamic_cast<Reader*>(targetUser);
	if (targetReader == nullptr) {
		throw std::runtime_error("User does not have shelves!");
	}

	const Shelf* shelf = targetReader->findShelf(shelfName);
	if (shelf == nullptr) {
		throw std::runtime_error("Shelf not found for this user!");
	}

	return shelf;
}

void BookService::publish(Publisher* currentPublisher, const std::string& bookTitle, const std::string& authorUsername, const Date& releaseDate, size_t pageCount, const std::vector<std::string>& genres)
{
	if (bookRepo.titleExists(bookTitle)) {
		throw std::invalid_argument("Book with this title already exists.");
	}

	User* authorUser = userRepo.getMutable(authorUsername);
	if (!authorUser) {
		throw std::runtime_error("User not found!");
	}

	Author* author = dynamic_cast<Author*>(authorUser);
	if (!author) {
		throw std::invalid_argument("User is not an author.");
	}

	bookRepo.add(std::make_unique<Book>(bookTitle, authorUsername, currentPublisher->getUsername(), releaseDate, pageCount, genres));

	author->addPublishedTitle(bookTitle);
	currentPublisher->addPublishedTitle(bookTitle);

	if (!author->hasPublisher(currentPublisher->getUsername())) {
		author->addPublisher(currentPublisher->getUsername());
		currentPublisher->addAuthor(authorUsername);
	}

	notifyFollowers(authorUsername, currentPublisher->getUsername(), bookTitle);
}

void BookService::notifyFollowers(const std::string& authorUsername, const std::string& publisherUsername, const std::string& bookTitle)
{
	auto allUsers = userRepo.getAllMutable();

	const User* author = userRepo.findByUsername(authorUsername);    
	const User* publisher = userRepo.findByUsername(publisherUsername);

	for (User* user : allUsers) {
		const std::string& username = user->getUsername();

		if (username == authorUsername || username == publisherUsername) {
			continue;
		}

		bool followsAuthor = author && author->isFollowedBy(username);        
		bool followsPublisher = publisher && publisher->isFollowedBy(username);

		if (!followsAuthor && !followsPublisher) {
			continue;
		}

		std::string content;
		if (followsAuthor && followsPublisher) {
			content = "New book published by author " + authorUsername + " and publisher " + publisherUsername + ": " + bookTitle;
		}
		else if (followsAuthor) {
			content = "New book published by author " + authorUsername + ": " + bookTitle;
		}
		else {
			content = "New book published by publisher " + publisherUsername + ": " + bookTitle;
		}

		user->receiveMessage(Message(publisherUsername, username, content, MessageType::BOOK_NOTIFY));
	}
}

void BookService::addSynopsis(Publisher* currentPublisher, const std::string& bookTitle, const std::string& synopsis)
{
	Book* book = bookRepo.getMutable(bookTitle);
	if (book == nullptr) {
		throw std::runtime_error("Book not found!");
	}

	if (book->getPublisherName() != currentPublisher->getUsername()) {
		throw std::invalid_argument("You are not the publisher of this book!");
	}

	book->setSynopsis(synopsis);
}
