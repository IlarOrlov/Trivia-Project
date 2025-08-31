#pragma once

#include "Communicator.h"


class Server
{
public:

	// methods for singleton
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;

	static Server& getInstance()
	{
		static Server instance;

		return instance;
	}

	// Methods
	void run();


private:

	// Constructors
	Server();


	// Fields
	std::shared_ptr<IDatabase> _database;
	RequestHandlerFactory* _handlerFactory;
	Communicator* _communicator;

};

