#define _CRT_SECURE_NO_WARNINGS
#include "utils/DateUtils.h"
#include "models/Date.h"
#include "constants/DateResources.h"
#include <ctime>
#include <chrono>

bool DateUtils::isLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int DateUtils::daysInMonth(int month, int year)
{
    if (month == resources::date::FEBRUARY && isLeapYear(year))
    {
        return resources::date::FEBRUARY_LEAP_DAYS;
    }

    return resources::date::MONTH_DAYS[month - 1];
}

Date DateUtils::today()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&t);

    return Date(localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
}
