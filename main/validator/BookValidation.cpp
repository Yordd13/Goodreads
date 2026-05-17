#include "validator/BookValidation.h"
#include "constants/BookResources.h"
#include <vector>

bool bookValidation::isValidTitle(const std::string& title)
{
	return !title.empty();
}

bool bookValidation::isValidAuthorName(const std::string& authorName)
{
	return !authorName.empty();
}

bool bookValidation::isValidPageCount(size_t pageCount)
{
	return pageCount >= resources::book::MIN_PAGE_COUNT;
}

bool bookValidation::isValidGenres(const std::vector<std::string>& genres)
{
	return !genres.empty();
}

bool bookValidation::isValidSynopsis(const std::string& synopsis)
{
	return synopsis.size() <= resources::book::MAX_SYNOPSIS;
}

bool bookValidation::isValidRating(int rating)
{
	return rating >= resources::book::MIN_RATING 
		&& rating <= resources::book::MAX_RATING;
}
