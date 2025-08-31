#pragma once

#include <ctime>
#include <vector>
#include "jsonUtils.h"

using JsonUtils::Buffer;


enum class REQUEST_ID 
{
	NONE, LOGIN, SIGNUP, CREATE_ROOM, JOIN_ROOM, GET_PLAYERS_IN_ROOM, GET_HIGH_SCORE, GET_PLAYER_STATISTICS, LOGOUT, GET_ROOMS , CLOSE_ROOM , START_ROOM, 
	GET_ROOM_STATE , LEAVE_ROOM
};

struct RequestInfo
{
	
	REQUEST_ID _id;
	time_t _receivalTime;
	Buffer _buffer;

};