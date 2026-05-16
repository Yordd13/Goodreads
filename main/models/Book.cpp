#include "models/Book.h"

Book::Book(const std::string& title, const std::string& authorName, 
    const std::string& publisherName, const Date& releaseDate, int pageCount, 
	const std::vector<std::string>& genres)
{
	//TODO: Validate input parameters
}

std::string Book::getTitle() const
{
    return title;
}

std::string Book::getAuthorName() const
{
    return authorName;
}

std::string Book::getPublisherName() const
{
    return publisherName;
}

std::string Book::getSynopsis() const
{
    return sinopsis;
}

Date Book::getReleaseDate() const
{
    return publicationDate;
}

size_t Book::getPageCount() const
{
    return numberOfPages;
}

double Book::getAvgRating() const
{
    return averageRating;
}

size_t Book::getRatingCount() const
{
    return numberOfRatings;
}

const std::vector<std::string>& Book::getGenres() const
{
    return genres;
}

void Book::setSynopsis(const std::string& synopsis)
{
	//TODO: Validate synopsis length
}

void Book::addRating(int rating)
{
	//TODO: Validate rating 1-10
	updateAverageRating(rating);
	updateRatingCount();
}

void Book::updateAverageRating(int newRating)
{
	averageRating = (averageRating * numberOfRatings + newRating) / (numberOfRatings + 1);
}

void Book::updateRatingCount()
{
	numberOfRatings++;
}

void Book::toSerial(std::ostream& out) const
{
}

Book Book::fromSerial(std::istream& in)
{
    return Book();
}
