#pragma once

#include <exception>
#include <string>


class MyException : public std::exception
{

public:
	MyException(const std::string& message) : _message(message) {}
	virtual ~MyException() noexcept = default;
	
	virtual const char* what() const noexcept { return _message.c_str(); }

protected:
	std::string _message;

};