#pragma once

#include <iostream>
#include "Server.h"
#include <string>
#include "JsonResponsePacketSerializer.h"

using std::exception;
using std::cout;
using std::endl;

int main()
{
	cout << "     ~ T R I V I A ~  			 " << endl
		 << "Ilar Orlov | Nikol Azarov  \n\n" << endl
		 << "PORT -> 4269\n" << endl; 

	try {
		(&Server::getInstance())->run();
		
	} catch (const exception& e) {
		cout << e.what() << endl;
	}
}