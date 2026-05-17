#pragma once
#include <vector>
#include <string>

namespace bookValidation
{
	bool isValidTitle(const std::string& title);
	bool isValidAuthorName(const std::string& authorName);
	bool isValidPageCount(size_t pageCount);
	bool isValidGenres(const std::vector<std::string>& genres);
	bool isValidSynopsis(const std::string& synopsis);
	bool isValidRating(int rating);
};

