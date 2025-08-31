#pragma once

#include "MyException.h"


class SqliteException : public MyException
{

public:
	SqliteException(const std::string& problem) :
		MyException("Sqlite Exception: " + problem) {}

};