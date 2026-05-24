#include "models/Book.h"
#include "validator/BookValidation.h"
#include "constants/BookResources.h"
#include <stdexcept>
#include <iostream>

void Book::setTitle(const std::string& title)
{
    if(bookValidation::isValidTitle(title)){
        this->title = title;
    }
    else {
		throw std::invalid_argument("Invalid title");
    }
}

void Book::setAuthorName(const std::string& authorName)
{
    if(bookValidation::isValidAuthorName(authorName)){
        this->authorName = authorName;
    }
    else {
        throw std::invalid_argument("Invalid author name");
	}
}

void Book::setGenres(const std::vector<std::string>& genres)
{
    if(bookValidation::isValidGenres(genres)){
        this->genres = genres;
    }
    else {
        throw std::invalid_argument("Book must have at least one genre");
	}
}

void Book::setPageCount(size_t pageCount)
{
    if(bookValidation::isValidPageCount(pageCount)){
        this->numberOfPages = pageCount;
    }
    else {
        throw std::invalid_argument("Invalid page count");
	}
}

Book::Book(const std::string& title, const std::string& authorName,
    const std::string& publisherName, const Date& releaseDate, size_t pageCount,
	const std::vector<std::string>& genres) : publicationDate(releaseDate), publisherName(publisherName), averageRating(0), numberOfRatings(0), sinopsis("")
{
    setTitle(title);
	setAuthorName(authorName);
	setPageCount(pageCount);
	setGenres(genres);
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
	if(bookValidation::isValidSynopsis(synopsis)){
        this->sinopsis = synopsis;
    }
    else {
        throw std::invalid_argument("Synopsis is too long");
	}
}

void Book::addRating(int rating)
{
	if(bookValidation::isValidRating(rating)){
        updateAverageRating(rating);
        updateRatingCount();
    }
    else {
        throw std::invalid_argument("Invalid rating");
	}
}

void Book::updateAverageRating(size_t newRating)
{
	averageRating = (averageRating * numberOfRatings + newRating) / (numberOfRatings + 1);
}

void Book::updateRatingCount()
{
	numberOfRatings++;
}

void Book::toSerial(std::ostream& out) const
{
    //   title
    //   authorName
    //   publisherName
    //   DD MM YYYY
    //   numberOfPages
    //   averageRating numberOfRatings
    //   genreCount genre1 genre2
    //   synopsisLength
    //   synopsis

    out << title << '\n'
        << authorName << '\n'
        << publisherName << '\n';

	out << publicationDate.toSerial() << '\n';

    out << numberOfPages << '\n'
        << averageRating << ' ' << numberOfRatings << '\n';

    out << genres.size() << '\n';
    for (const auto& g : genres) {
        out << g << '\n';
    }

    out << sinopsis.size() << '\n';
    if (!sinopsis.empty()) {
        out << sinopsis << '\n';
    }
}

Book Book::fromSerial(std::istream& in)
{
    std::string title, authorName, publisherName;
    std::getline(in, title);
    std::getline(in, authorName);
    std::getline(in, publisherName);


	int serialDate;
	in >> serialDate;
    Date releaseDate = Date::fromSerial(serialDate);
    in.ignore();

    size_t pageCount;
    in >> pageCount;

    double avgRating;
    size_t ratingCount;
    in >> avgRating >> ratingCount;

    size_t genreCount;
    in >> genreCount;
    in.ignore();

    std::vector<std::string> genres;
    genres.reserve(genreCount);
    for (int i = 0; i < genreCount; i++){
        std::string g;
        std::getline(in, g);
        genres.push_back(g);
    }

    size_t synopsisLen;
    in >> synopsisLen;
    in.ignore();

    std::string synopsis;
    if (synopsisLen > 0) {
        std::getline(in, synopsis);
    }

    Book book(title, authorName, publisherName, releaseDate, pageCount, genres);

    book.averageRating = avgRating;
    book.numberOfRatings = ratingCount;
    if (!synopsis.empty()) {
        book.sinopsis = synopsis;
    }

    return book;
}
