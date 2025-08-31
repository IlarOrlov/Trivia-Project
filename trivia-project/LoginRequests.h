#pragma once

#include "Address.h"
#include <string>


struct LoginRequest
{
	std::string _username, 
				_password;
};

struct SignupRequest
{
	std::string _username,
				_password,
				_email,
				_phone,
				_date;
	Address		_address;
};