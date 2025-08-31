#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class RoomAdminRequestHandler : public IRequestHandler
{

public:

	RoomAdminRequestHandler(RequestHandlerFactory* handleFactory);
	virtual ~RoomAdminRequestHandler() = default;
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;


private:
	
	RequestHandlerFactory* _handleFactory;
	RequestResult closeRoom(const Buffer& buffer);
	RequestResult startRoom(const Buffer& buffer);
	RequestResult getRoomState(const Buffer& buffer);


};

