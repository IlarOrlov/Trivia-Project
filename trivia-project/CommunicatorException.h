#pragma once

#include "MyException.h"


class CommunicatorException : public MyException
{

public:
	CommunicatorException(const std::string& problem) :
		MyException("The communicator went throw a problem: " + problem) {}

};