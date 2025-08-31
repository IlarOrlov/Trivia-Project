#pragma once

#include <vector>
#include "json.hpp"


using json = nlohmann::json;


namespace JsonUtils 
{

	typedef std::vector<char> Buffer;


	constexpr int BYTE_SIZE = 8;
	constexpr int DATA_LENGTH = 32;
	const std::string CODE = "00000000";

}