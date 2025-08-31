#include "MyException.h"


class RoomException : public MyException
{

public:
	RoomException(const std::string& problem) :
		MyException(problem) {}

};