#pragma once
#include <system_error>
#include <stdexcept>
#include <string>
#include <cstdlib>

class CritError : public std::runtime_error
{
public:
    CritError(const std::string& message) : std::runtime_error(message) {}
};

class NoCritError : public std::runtime_error
{
public:
    NoCritError(const std::string message) : std::runtime_error(message) {}
};
