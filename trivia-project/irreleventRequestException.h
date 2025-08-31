#pragma once

#include "MyException.h"


class IrreleventRequestException : public MyException
{

public:
	IrreleventRequestException() :
		MyException("Invalid Request") {}

};