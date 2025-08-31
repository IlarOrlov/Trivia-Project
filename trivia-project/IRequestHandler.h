#pragma once

#include "RequestInfo.h"
#include "RequestResult.h"

struct RequestResult;

class IRequestHandler 
{

public:

	// Methods
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};