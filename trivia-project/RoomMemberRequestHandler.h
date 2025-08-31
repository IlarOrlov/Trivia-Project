#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class RoomMemberRequestHandler : public IRequestHandler
{


public:

	// Constructors
	RoomMemberRequestHandler(RequestHandlerFactory* handleFactory);
	virtual ~RoomMemberRequestHandler() = default;
	
	// Methods
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;


private:

	// Methods
	RequestResult leaveRoom(const Buffer& buffer);
	RequestResult getRoomState(const Buffer& buffer);

	// Fields
	RequestHandlerFactory* _handleFactory;

};

