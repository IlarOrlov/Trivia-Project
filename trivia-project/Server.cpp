#pragma comment (lib, "ws2_32.lib")

#include "Server.h"
#include "SqliteDatabase.h"
#include <iostream>
#include <exception>

using std::cout;
using std::endl;
using std::cerr;
using std::exception;
using std::shared_ptr;

//? Constructors ?//

//? C'tor ?// 
Server::Server() :
	_database(SqliteDatabase::getInstance()),
	_handlerFactory(&RequestHandlerFactory::getInstance(_database)), 
	_communicator(&Communicator::getInstance(_handlerFactory))
{
}



//? Methods ?//


/* -- run the server, start communicator -- */
void Server::run()
{
	_communicator->run();
}