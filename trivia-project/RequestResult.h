#pragma once

#include <string>
#include "IRequestHandler.h"
#include "JsonUtils.h"
#include <memory>

class IRequestHandler;

using JsonUtils::Buffer;


struct RequestResult 
{
	
	Buffer _response;
	std::shared_ptr<IRequestHandler> _newHandler;
	
};