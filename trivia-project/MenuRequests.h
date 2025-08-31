#pragma once
#include <string>
#include "RoomData.h"


struct JoinRoomRequest
{
	std::string _roomName;
	std::string _user;
};


struct CreateRoomRequest
{
	RoomData _roomData;
};


struct GetPlayersInRoomRequest
{
	std::string _roomName;
};
