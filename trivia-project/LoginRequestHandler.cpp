#include "LoginRequestHandler.h"
#include "irreleventRequestException.h"
#include "LoginException.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "MenuRequestHandler.h"

#include <memory>

using std::shared_ptr;
using std::string;



//? Constructors ?//


/* -- c'tor -- */
LoginRequestHandler::LoginRequestHandler( LoginManager* loginManager, 
										  RequestHandlerFactory* factory) :
	_loginManager(loginManager), _handleFactory(factory)
{
}



//? Methods ?//


/*
	-- find if the request is relevant or not --
	* input: requestInfo
	* output: is tequest relevent or not
*/
bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{

	return  requestInfo._id == REQUEST_ID::LOGIN || 
			requestInfo._id == REQUEST_ID::SIGNUP;

}

/*
	-- handle the current request - analyze the request info 
	   and return a suited response --
	* input: current request's info
	* output: request's result
*/
RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{

	try {
	
		if ( requestInfo._id == REQUEST_ID::LOGIN )
			return handleLoginRequest(requestInfo._buffer);
		
		if ( requestInfo._id == REQUEST_ID::SIGNUP )
			return handleSignupRequest(requestInfo._buffer); 
		return handleBadRequest();

	}
	catch (const LoginException& loginException) {
		return handleBadRequest(loginException.what());
	
	} catch (const std::exception& exception) {
		return handleBadRequest(exception.what(), false);
	}

}

/*
	-- handle signup request --
	* input: buffer
	* output: request's result
*/
RequestResult LoginRequestHandler::handleSignupRequest(const Buffer& buffer)
{

	// deserialize the message from json format 
	SignupRequest request = JsonRequestPacketDeserializer::deserializeSignupRequest(buffer);

	std::shared_ptr<IRequestHandler> nextHandler;


	_loginManager->signup(request);

	return {
		JsonResponsePacketSerializer::serializeResponse(OkResponse()),
		_handleFactory->createMenuRequestHandler()
	};
	
}

/*
	-- handle login request --
	* input: buffer
	* output: request's result
*/
RequestResult LoginRequestHandler::handleLoginRequest(const Buffer& buffer)
{

	// deserialize the message from json format 
	LoginRequest request = JsonRequestPacketDeserializer::deserializeLoginRequest(buffer);


	_loginManager->login(request);

	return {
		JsonResponsePacketSerializer::serializeResponse(OkResponse()),
		_handleFactory->createMenuRequestHandler()
	};


}

RequestResult LoginRequestHandler::handleBadRequest(const string& errorMessage,
	const bool returnLoginRequest)
{

	return {
		JsonResponsePacketSerializer::serializeResponse(ErrorResponse{errorMessage}),
		(returnLoginRequest ? _handleFactory->createLoginRequestHandler() : nullptr)
	};

}
