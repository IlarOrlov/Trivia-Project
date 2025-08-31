#pragma once

#include "SqliteDatabase.h"
#include <boost/asio/dispatch.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include "clientSocket.h"
#include "RequestHandlerFactory.h"
#include "JsonUtils.h"
#include <string>


using namespace JsonUtils;
using beast::buffers_to_string;


class Communicator
{

public:
	
	// methods for singleton
	Communicator(Communicator const&) = delete;
	void operator=(Communicator const&) = delete;
	static Communicator& getInstance(RequestHandlerFactory* handlerFactory)
	{
		static Communicator instance(handlerFactory);

		return instance;
	}


	// Methods
	void run();


	// Socket Methods
	void handleNewClient(ClientSocket socket);
	void listenToConsoleForExitMessage();

	//	 Analyze Client Message Methods
	RequestInfo read(ClientSocket& socket);
	void send(ClientSocket& socket, Buffer& buffer);

	ClientSocket accept();


private:

	// Constructors
	Communicator(RequestHandlerFactory* handlerFactory);

	void print(const std::string& output);

	// Fields
	static constexpr const char* IP = "127.0.0.1";
	static constexpr int PORT = 4269;

	static constexpr int MESSAGE_CODE_SIZE = BYTE_SIZE * 1;
	static constexpr int MESSAGE_LENGTH_SIZE = BYTE_SIZE * 4;
	static constexpr const char* EXIT_MESSAGE = "EXIT";


	net::io_context _ioc;
	tcp::acceptor _acceptor;

	std::mutex _printMutex;

	RequestHandlerFactory* _handlerFactory;
	
	bool _exit;

};




