#include "DateValidation.h"
#include "DateResources.h"

bool dateValidation::isValid(int day, int month, int year)
{
    return isValidYear(year) 
        && isValidMonth(month) 
        && isValidDay(day, month, year);
}

bool dateValidation::isValidYear(int year)
{
    return year >= resources::date::MIN_YEAR 
        && year <= resources::date::MAX_YEAR;
}

bool dateValidation::isValidMonth(int month)
{
    return month >= resources::date::MIN_MONTHS 
        && month <= resources::date::MAX_MONTHS;
}

bool dateValidation::isValidDay(int day, int month, int year)
{
    return day >= resources::date::MIN_DAY 
        && day <= resources::date::MONTH_DAYS[month - 1];
}
