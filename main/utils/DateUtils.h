#pragma once

class Date;

namespace DateUtils
{
	bool isLeapYear(int year);
	int daysInMonth(int month, int year);

	Date today();
};

