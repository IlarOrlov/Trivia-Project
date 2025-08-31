#pragma once

#include "JsonUtils.h"
#include "GlobalResponses.h"
#include "MenuResponses.h"
#include "StatisticsResponses.h"
#include "GameResponses.h"

#include <vector>
#include <sstream>

using namespace JsonUtils;


class JsonResponsePacketSerializer
{


public:

	// Methods for singleton
	JsonResponsePacketSerializer(JsonResponsePacketSerializer const&) = delete;
	void operator=(JsonResponsePacketSerializer const&) = delete;

	static JsonResponsePacketSerializer& getInstance()
	{
		static JsonResponsePacketSerializer instance;

		return instance;
	}


	// Methods

	//    OK response
	static Buffer serializeResponse(const OkResponse& response);

	//	  Room
	static Buffer serializeResponse(const GetRoomsResponse& response);
	static Buffer serializeResponse(const GetPlayersInRoomResponse& response);
	static Buffer serializeResponse(const GetRoomStateRespone& response);

	//	 Statistics
	static Buffer serializeResponse(const getHighScoreResponse& response);
	static Buffer serializeResponse(const getPersonalStatsResponse& response);


	static Buffer serializeResponse(const ErrorResponse& response);


private:
	// constructors
	JsonResponsePacketSerializer() = default;

	static Buffer formatMessage(const std::string& data);

	template<class T1, class T2>
	inline static std::string makeJsonPair(const T1& key, const T2& val);
	template<class T>
	inline static std::string makeJsonPair(const T& key, const bool val);
};


template<class T1, class T2>
inline std::string JsonResponsePacketSerializer::makeJsonPair(const T1& key, const T2& val)
{
	std::stringstream s;
	s << "\"" << key << "\" : \"" << val << "\"";
	return s.str();
}

template<class T>
inline static std::string JsonResponsePacketSerializer::makeJsonPair(const T& key, const bool val) {
	return makeJsonPair(key, val ? "True" : "False");
}
