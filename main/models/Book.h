#pragma once
#include <string>
#include <vector>
#include "models/Date.h"

class Book
{
private:

	std::string title;
	std::string authorName;
	std::string publisherName;
	std::string sinopsis;
	std::vector<std::string> genres;
	double averageRating;
	size_t numberOfRatings;
	Date publicationDate;
	size_t numberOfPages;

	void setTitle(const std::string& title);
	void setAuthorName(const std::string& authorName);
	void setGenres(const std::vector<std::string>& genres);
	void setPageCount(size_t pageCount);

public:

	Book() = default;
	Book(const std::string& title, const std::string& authorName,
		const std::string& publisherName, const Date& releaseDate,
		size_t pageCount, const std::vector<std::string>& genres);

	std::string getTitle()         const;
	std::string getAuthorName()    const;
	std::string getPublisherName() const;
	std::string getSynopsis()      const;
	Date        getReleaseDate()   const;
	size_t      getPageCount()     const;
	double      getAvgRating()     const;
	size_t      getRatingCount()   const;
	const std::vector<std::string>& getGenres() const;

	void setSynopsis(const std::string& synopsis);

	void addRating(int rating);

private:

	void updateAverageRating(size_t newRating);
	void updateRatingCount();

	void toSerial(std::ostream& out) const;
	static Book fromSerial(std::istream& in);

};

