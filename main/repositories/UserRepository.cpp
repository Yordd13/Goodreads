#include "repositories/UserRepository.h"
#include <fstream>
#include <stdexcept>
#include "users/Reader.h"
#include "users/Author.h"
#include "users/Publisher.h"
#include <algorithm>

UserRepository::UserRepository(const std::string& filePath) : filePath(filePath)
{
	loadFromFile();
}

UserRepository::~UserRepository()
{
	saveToFile();
}

const User* UserRepository::findByUsername(const std::string& username) const
{
	return findWhere([&username](const User& u) {
		return u.getUsername() == username;
		});
}

bool UserRepository::usernameExists(const std::string& username) const
{
	return findWhere([&username](const User& u){
		return u.getUsername() == username;
	}) != nullptr;
}

std::vector<const User*> UserRepository::getFollowersOf(const std::string& username) const
{
	const User* target = findByUsername(username);    
	if (!target) {
		return {};
	}
	
	std::vector<const User*> result;    
	for (const auto& f : target->getFollowers()) {        
		if (const User* u = findByUsername(f)) {            
			result.push_back(u);        
		}    
	}   

	return result;
}

std::vector<const User*> UserRepository::getFriendsOf(const std::string& username) const
{
	const User* target = findByUsername(username);
	if (!target) {
		return {}; 
	}

	std::vector<const User*> result;    
	for (const auto& followerName : target->getFollowers()) { 
		const User* candidate = findByUsername(followerName);        
		if (candidate && candidate->isFollowedBy(username)) { 
			result.push_back(candidate); 
		} 
	}    
	return result;
}

User* UserRepository::getMutable(const std::string& username)
{
	return findMutable([&username](const User& u) {
		return u.getUsername() == username;
		});
}

std::vector<User*> UserRepository::getAllMutable()
{
	return findAllMutable([](const User&) {
		return true; 
	});
}

std::vector<const User*> UserRepository::fuzzySearchByUsername(const std::string& query) const
{
	std::string lowerQuery = query;
	std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return findAllWhere([&lowerQuery](const User& u) {
		std::string lowerName = u.getUsername();
		std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return editDistance(lowerQuery, lowerName) <= 2;
		});
}


int UserRepository::editDistance(const std::string& a, const std::string& b)
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


void UserRepository::loadFromFile()
{
	std::ifstream in(filePath);
	if (!in.is_open()) {
		return;
	}

	size_t count;
	in >> count;
	in.ignore();

	for (size_t i = 0; i < count; i++){
		std::string type;
		std::getline(in, type);

		std::unique_ptr<User> user;
		if (type == "Reader") {
			user = std::make_unique<Reader>();
		}
		else if (type == "Author") {
			user = std::make_unique<Author>();
		}
		else if (type == "Publisher") {
			user = std::make_unique<Publisher>();
		}
		else {
			throw std::runtime_error("UserRepository: unknown user type in file!");
		}

		user->deserialize(in);
		data.push_back(std::move(user));
	}
}

void UserRepository::saveToFile() const
{
	std::ofstream out(filePath);
	if (!out.is_open()) {
		throw std::runtime_error("UserRepository: cannot open file for writing!");
	}

	out << data.size() << '\n';
	for (const auto& user : data){
		user->serialize(out);
	}
}
