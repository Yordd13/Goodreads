#include "repositories/BookRepository.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

BookRepository::BookRepository(const std::string& filePath) : filePath(filePath)
{
    loadFromFile();
}

BookRepository::~BookRepository()
{
    saveToFile();
}


const Book* BookRepository::findByTitle(const std::string& title) const
{
    return findWhere([&title](const Book& b) {
        return b.getTitle() == title;
    });
}

Book* BookRepository::getMutable(const std::string& title)
{
    return findMutable([&title](const Book& b) {
        return b.getTitle() == title;
    });
}

bool BookRepository::titleExists(const std::string& title) const
{
    return findByTitle(title) != nullptr;
}

std::vector<const Book*> BookRepository::findByAuthor(const std::string& authorName) const
{
    return findAllWhere([&authorName](const Book& b) {
        return b.getAuthorName() == authorName;
    });
}

std::vector<const Book*> BookRepository::findByPublisher(const std::string& publisherName) const
{
    return findAllWhere([&publisherName](const Book& b) {
        return b.getPublisherName() == publisherName;
    });
}

std::vector<const Book*> BookRepository::findByGenre(const std::string& genre) const
{
    return findAllWhere([&genre](const Book& b) {
        const auto& genres = b.getGenres();
        return std::find(genres.begin(), genres.end(), genre) != genres.end();
    });
}
std::vector<const Book*> BookRepository::fuzzySearchByTitle(const std::string& query) const
{
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
        [](unsigned char c) { return std::tolower(c); });

    return findAllWhere([&lowerQuery](const Book& b) {
        std::string lowerTitle = b.getTitle();
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(),
            [](unsigned char c) { return std::tolower(c); });

        return editDistance(lowerQuery, lowerTitle) <= 2;
    });
}


int BookRepository::editDistance(const std::string& a, const std::string& b)
{
    const size_t m = a.size();
    const size_t n = b.size();

    if (m > n + 2 || n > m + 2) return 3;

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (size_t i = 0; i <= m; i++) dp[i][0] = static_cast<int>(i);
    for (size_t j = 0; j <= n; j++) dp[0][j] = static_cast<int>(j);

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
    }

    return dp[m][n];
}

void BookRepository::loadFromFile()
{
    std::ifstream in(filePath);
    if (!in.is_open()) {
        return;
    }

    size_t count;
    in >> count;
    in.ignore();

    for (size_t i = 0; i < count; i++) {
        data.push_back(std::make_unique<Book>(Book::fromSerial(in)));
    }
}

void BookRepository::saveToFile() const
{
    std::ofstream out(filePath);
    if (!out.is_open()) {
        throw std::runtime_error("BookRepositorycannot open file to write!");
    }

    out << data.size() << '\n';
    for (const auto& book : data) {
        book->toSerial(out);
    }
}