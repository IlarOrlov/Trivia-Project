#pragma once

#include <string>
#include <unordered_map>
#include <set>
#include "Room.h"
#include "RoomData.h"


struct GetRoomsResponse
{
	std::unordered_map<std::string, Room> _rooms;
};


struct GetPlayersInRoomResponse
{
	std::set<std::string> _players;
};
