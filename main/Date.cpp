#include "Date.h"
#include "DateValidation.h"
#include "DateResources.h"
#include "DateUtils.h"
#include <stdexcept>
#include <chrono>
#include <iomanip>


Date::Date(int day, int month, int year)
{
	if (!dateValidation::isValid(day, month, year)) {
		throw std::invalid_argument("Invalid date");
	}

	this->day = day;
	this->month = month;
	this->year = year;
}

int Date::getDay() const
{
	return day;
}

int Date::getMonth() const
{
	return month;
}

int Date::getYear() const
{
	return year;
}

Date Date::fromString(const std::string& str)
{
	int d, m, y;
	char sep1, sep2;

	std::istringstream iss(str);
	iss >> d >> sep1 >> m >> sep2 >> y;

	if (iss.fail() || sep1 != '/' || sep2 != '/') {
		throw std::invalid_argument("Invalid date format");
	}

	return Date(d, m , y);
}

std::string Date::toString() const
{
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << day << '/'
		<< std::setw(2) << std::setfill('0') << month << '/'
		<< std::setw(4) << std::setfill('0') << year;

	return oss.str();
}


int Date::toSerial() const
{
	int serial = 0;

	for (int y = resources::date::MIN_YEAR; y < year; ++y) {
		serial += DateUtils::isLeapYear(y) ? 366 : 365;
		if (DateUtils::isLeapYear(y)) {
			serial += 1;
		}
	}

	for (int m = 1; m < month; ++m) {
		serial += DateUtils::daysInMonth(m, year);
	}

	serial += day - 1;

	return serial;
}

Date Date::fromSerial(int serial)
{
	int serialYear = resources::date::MIN_YEAR;
	while (true)
	{
		int yearLen = resources::date::DAYS_IN_YEAR;
		if (DateUtils::isLeapYear(serialYear))
		{
			yearLen += 1;
		}

		if (serial < yearLen)
		{
			break;
		}

		serial -= yearLen;
		serialYear++;
	}

	int serialMonth = 1;
	while (true)
	{
		int monthLen = DateUtils::daysInMonth(serialMonth, serialYear);

		if (serial < monthLen)
		{
			break;
		}

		serial -= monthLen;
		serialMonth++;
	}

	int serialDay = serial + 1;
	return Date(serialDay, serialMonth, serialYear);
}