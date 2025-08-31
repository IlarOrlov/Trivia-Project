#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

#include <boost/beast/websocket.hpp>
#include <boost/beast/core.hpp>
#include <memory>


namespace beast = boost::beast;
namespace http = beast::http;
namespace ws = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

typedef ws::stream<tcp::socket> WebSocket;



class ClientSocket
{

public:

	// constructors
	ClientSocket(WebSocket& socket, RequestHandlerFactory* factory);

	
	// getters
	unsigned int getId() const { return _id; };
	WebSocket& getSocket() { return _socket; };
	std::shared_ptr<IRequestHandler>& getHandler() { return _handler; };

	// setters
	void setHandler(std::shared_ptr<IRequestHandler>& handler) { _handler = handler; };
	

	// operators
	bool operator<(const ClientSocket& other) const { return _id < other._id; };

	
	// public fields
	static unsigned int nextId;


private:

	// private fields
	const unsigned int _id = nextId++;
	std::shared_ptr<IRequestHandler> _handler;
	WebSocket _socket;

};