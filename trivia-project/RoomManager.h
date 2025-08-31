#pragma once
#include "Room.h"
#include "RoomRequests.h"
#include "MenuRequests.h"
#include <unordered_map>


class RoomManager
{

public:
	
	void createRoom(const RoomData& roomData);
	void closeRoom(const std::string& roomName);
	
	void joinRoom(const JoinRoomRequest&);
	void leaveRoom(const LeaveRoomRequest& req);

	RoomData getRoomData(const std::string& roomName);
	std::set<std::string> getPlayersInRoom(const std::string& name);
	std::unordered_map<std::string, Room> GetRooms();
	void activateRoom(const std::string& roomName);

private:

	std::unordered_map<std::string, Room> _rooms;
	
};

