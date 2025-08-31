#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <iostream>



class MenuRequestHandler : public IRequestHandler
{

public:

	// Constructors
	MenuRequestHandler(RequestHandlerFactory* handleFactory);
	virtual ~MenuRequestHandler()  = default;

	// Methods
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;


private:

	// fields
	RequestHandlerFactory* _handleFactory;
	
	
	// Private Methods
	RequestResult createRoom(const Buffer& buffer);
	RequestResult getRooms(const Buffer& buffer);
	RequestResult getPlayersInRoom(const Buffer& buffer);
	RequestResult joinRoom(const Buffer& buffer);
	RequestResult getStatistics(const Buffer& buffer);
	RequestResult getHighScore(const Buffer& buffer);
	RequestResult logout(const Buffer& buffer);


};

