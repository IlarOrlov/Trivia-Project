#include "ClientSocket.h"
#include "LoginRequestHandler.h"

unsigned int ClientSocket::nextId = 1;



/* -- c'tor -- */
ClientSocket::ClientSocket(WebSocket& socket, RequestHandlerFactory* factory) :
	_socket(std::move(socket)), _handler(new LoginRequestHandler(factory->getLoginManager(), factory))
{
}
