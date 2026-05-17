#pragma once
#include <string>

class Date
{
private:

	int year;
	int month;
	int day;

public:

    Date() = default;
    Date(int day, int month, int year);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    static Date fromString(const std::string& str);
    std::string toString() const;

    auto operator<=>(const Date& other) const = default;
    bool operator==(const Date& other) const = default;

    int toSerial() const;
    static Date fromSerial(int serial);
};

