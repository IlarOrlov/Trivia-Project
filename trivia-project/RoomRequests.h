#pragma once

#include "RoomData.h"
#include <string>


struct CloseRoomRequest
{
	std::string _roomName;
};

struct LeaveRoomRequest
{
	std::string _user,
		        _roomName;
};

struct RoomStateRequest
{
	std::string _roomName;
};


struct StartRoomRequest
{
	std::string _roomName;
};