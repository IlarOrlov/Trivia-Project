#include "Communicator.h"
#include "LoginRequestHandler.h"
#include "CommunicatorException.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestInfo.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std::string_literals;
using std::chrono::system_clock;
using std::exception;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::shared_ptr;
using std::make_shared;
using std::pair;
using std::string;
using std::to_string;
using std::atoi;
using std::cout;
using std::endl;


//? Constructors ?//


/* -- c'tor -- */
Communicator::Communicator(RequestHandlerFactory* handlerFactory) :
	_ioc(INT16_MAX), _acceptor(_ioc, {net::ip::make_address(IP), PORT}),
	_handlerFactory(handlerFactory), _exit(false)
{

	// create a listening thread to listen for the EXIT_MESSAGE
	thread (&Communicator::listenToConsoleForExitMessage, this).detach();

}




//? Public Methods ?//


/* -- start handle requests for clients -- */
void Communicator::run()
{

	while (!_exit)
		thread (&Communicator::handleNewClient, this, accept()).detach();
}




//? Private Methods ?//


/* -- accept new client -- */
ClientSocket Communicator::accept()
{

	tcp::socket tcpSocket{ _ioc };
	_acceptor.accept(tcpSocket);

	WebSocket socket { std::move(tcpSocket) };

	socket.set_option(ws::stream_base::decorator([](ws::response_type& res)
		{
			res.set(http::field::server, string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
		}));

	socket.accept();

	return ClientSocket(socket, _handlerFactory);

}


/*
	-- handle the new client --
	* input: client's socket (it's not reference on purpose)
*/
void Communicator::handleNewClient(ClientSocket socket)
{

	RequestInfo request;
	RequestResult result;
	
	bool _badInput = false;
	
	print("\nhello Client "s + to_string(socket.getId()) + "!");


	while (!_exit && !_badInput)
	{
		try {
				
			request = read(socket);
			result = socket.getHandler()->handleRequest(request);
			
			if (result._newHandler)
				socket.setHandler(result._newHandler);
			
			send(socket, result._response);


		} catch (exception& e) {
			print("client "s + to_string(socket.getId()) + " disconnected: " + e.what());
			_badInput = true;
		}
	}

}

/*
	-- get a new message from the client by the given socket --
	* input: connection socket
	* output: request's info
*/
RequestInfo Communicator::read(ClientSocket& socket)
{
	
	RequestInfo requestInfo;

	
	// read client msg
	beast::multi_buffer buffer;
	socket.getSocket().read(buffer);


	string data = buffers_to_string(buffer.data());


	// get current time 
	requestInfo._receivalTime = system_clock::to_time_t(system_clock::now());

	// get id 
	requestInfo._id = (REQUEST_ID)stoi(data.substr(0, MESSAGE_CODE_SIZE), 0, 2);

	// get buffer
	string helper = data.substr(MESSAGE_CODE_SIZE + MESSAGE_LENGTH_SIZE);
	requestInfo._buffer = Buffer(helper.begin(), helper.end());


	return requestInfo;

}



/*
	-- send the given buffer to the client's socket --
	* input: socket with client, buffer
*/
void Communicator::send(ClientSocket& socket, Buffer& buffer)
{

	socket.getSocket().write(net::buffer(string(buffer.begin(), buffer.end())));

}


/* -- listen to console for EXIT_MESSAGE -- */
void Communicator::listenToConsoleForExitMessage()
{

	string cmdInput;

	while (cmdInput != EXIT_MESSAGE)
		std::cin >> cmdInput;

	_exit = true;
	
}



/* -- print output (thread safe) -- */
void Communicator::print(const string& output)
{
	lock_guard<mutex> lock(_printMutex);
	cout << output << endl;
}