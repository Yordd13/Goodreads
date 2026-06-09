#include "utils/DateUtils.h"
#include "models/Date.h"
#include "constants/DateResources.h"
#include <ctime>

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
    std::time_t t = std::time(nullptr);
    std::tm now{};

    localtime_s(&now, &t);

    return Date(now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
}
