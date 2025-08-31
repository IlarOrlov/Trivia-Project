#include "MyException.h"


class LoginException : public MyException
{

public:
	LoginException(const std::string& problem) :
		MyException(problem) {}

};