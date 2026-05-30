#include "models/PersonalLibrary.h"
#include <iostream>

PersonalLibrary::PersonalLibrary(const std::string& title,
    ReadStatus status, int rating)
    : title(title), status(status), rating(rating) {
}

ReadStatus PersonalLibrary::statusFromInt(int statusInt)
{
    switch (statusInt){
    case 0: return ReadStatus::PLAN_TO_READ;
    case 1: return ReadStatus::READING;
    case 2: return ReadStatus::PAUSED;
    case 3: return ReadStatus::DROPPED;
    default:
        throw std::runtime_error("Invalid read status!");
    }
}

void PersonalLibrary::serialize(std::ostream& out) const
{
    out << static_cast<int>(status) << ' ' << rating << '\n';
    out << title << '\n';
}

PersonalLibrary PersonalLibrary::deserialize(std::istream& in)
{
    int statusInt, rating;
    in >> statusInt >> rating;
    in.ignore();

    std::string title;
    std::getline(in, title);

    return PersonalLibrary(title, statusFromInt(statusInt), rating);
}