#pragma once
#include <stdexcept>

class parse_error : public std::invalid_argument
{
public:
    explicit parse_error(const std::string what)
        : std::invalid_argument(what)
    { }
};