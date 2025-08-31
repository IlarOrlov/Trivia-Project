#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{

public:

	// Constructors
	LoginRequestHandler(LoginManager* loginManager, 
		RequestHandlerFactory* factory);
	virtual ~LoginRequestHandler() = default;

	// Methods
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;


private:

	// Fields
	LoginManager* _loginManager;
	RequestHandlerFactory* _handleFactory;


	// Private Methods
	RequestResult handleSignupRequest(const Buffer& requestBuffer);
	RequestResult handleLoginRequest(const Buffer& requestBuffer);
	RequestResult handleBadRequest( const std::string& errorMessage = "bad login",
									const bool returnLoginRequest = true);
	

};

