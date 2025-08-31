#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"

class LoginRequestHandler;

class RequestHandlerFactory
{

public:

	// methods for singleton
	RequestHandlerFactory(const RequestHandlerFactory&) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory&) = delete;

	static RequestHandlerFactory& getInstance(std::shared_ptr<IDatabase>& database)
	{
		static RequestHandlerFactory instance(database);

		return instance;
	}

	// Methods
	std::shared_ptr<IRequestHandler> createLoginRequestHandler();
	std::shared_ptr<IRequestHandler> createMenuRequestHandler();
	std::shared_ptr<IRequestHandler> createRoomAdminRequestHandler();
	std::shared_ptr<IRequestHandler> createRoomMemberRequestHandler();


	// Getters
	LoginManager* getLoginManager() { return _loginManager; };
	RoomManager* getRoomManager() { return _roomManager; };
	StatisticsManager* getStatisticsManager() { return _statisticsManager; };


private:

	// Constructors 
	RequestHandlerFactory(std::shared_ptr<IDatabase>& database);

	// Fields
	LoginManager* _loginManager;
	RoomManager* _roomManager;
	StatisticsManager* _statisticsManager;

	std::shared_ptr<IDatabase> _database;

};

