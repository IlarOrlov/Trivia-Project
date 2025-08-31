#pragma once
#include "RoomData.h"


struct GetRoomStateRespone
{
    RoomData _roomData;
    std::set<std::string> _users;
};