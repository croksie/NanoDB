#pragma once
#include <iostream>

class DatabaseException : public std::runtime_error
{
public:
	DatabaseException(char const* const message) throw();
	DatabaseException(const std::string message) throw();
	virtual char const* what() const throw();
};

