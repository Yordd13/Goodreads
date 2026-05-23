#pragma once
#include <string>

enum class ReadStatus
{
    PLAN_TO_READ,
    READING,
    PAUSED,
    DROPPED
};

struct PersonalLibrary
{
    std::string title;
    ReadStatus status;
    int rating;

    PersonalLibrary() = default;
    PersonalLibrary(const std::string& title, ReadStatus status, int rating = -1);

    void serialize(std::ostream& out)  const;
    static PersonalLibrary deserialize(std::istream& in);
};