#include "RoomManager.h"
#include "RoomException.h"
#include "RoomData.h"


using std::string;



//? Methods ?//


/*
	-- Adds  a room to the room manager--
	* input : the creator username of the room
	* output : None
*/
void RoomManager::createRoom(const RoomData& roomData)
{
	if (roomData._name == "")
		throw RoomException("Room Name Can't Be Null");
	
	_rooms[roomData._name] = Room(roomData);

}


/*
	-- Removes a room from the room manager --
	* input : room name
	* output : None
*/
void RoomManager::closeRoom(const string& roomName)
{
	if (!_rooms.erase(roomName))
		throw RoomException("Room doesn't exist");

}


/*
	-- Joins a room --
	* input : room name , user name 
	* output : None
*/
void RoomManager::joinRoom(const JoinRoomRequest& req)
{

	if (_rooms.find(req._roomName) == _rooms.end())
		throw RoomException("Room doesn't exist");
	

	auto maxPlayers = _rooms[req._roomName].getData()._maxPlayers;
	
	if (maxPlayers && maxPlayers == _rooms[req._roomName].getAllUsers().size())
		throw RoomException("Room is full");
	
	_rooms[req._roomName].addUser(req._user);

}


/*
	-- Leaves a room --
	* input : room name , user name
	* output : None
*/
void RoomManager::leaveRoom(const LeaveRoomRequest& req)
{

	if (_rooms.find(req._roomName) == _rooms.end())
		return; // dont change - for when member kicked out when room closed

	_rooms[req._roomName].removeUser(req._user);

}


/*
	-- Gets the state of a room --
	* input : room id
	* output : room state
*/
RoomData RoomManager::getRoomData(const string& roomName)
{
	if (_rooms.find(roomName) == _rooms.end())
		throw RoomException("Room doesn't exist");
	
	return _rooms[roomName].getData();

}


/* -- Gets the players in a room -- */
std::set<string> RoomManager::getPlayersInRoom(const string& roomName)
{
	if (_rooms.find(roomName) == _rooms.end())
		throw RoomException("Room doesn't exists");

	return _rooms[roomName].getAllUsers();
}


/* -- Gets the rooms -- */
std::unordered_map<string, Room> RoomManager::GetRooms()
{
	return _rooms;
}


/* -- Activate Room --*/
void RoomManager::activateRoom(const string& roomName)
{
	if (_rooms.find(roomName) == _rooms.end())
		throw RoomException("Room doesn't exists");
	
	_rooms[roomName].activate();
}