#include "Room.h"
#include "RoomException.h"


using std::string;


//? Constructor  ?//


/* -- Constructor -- */
Room::Room(const RoomData& roomData) : 
	_roomData(roomData)
{
	_players.insert(roomData._admin);
}


//? Methods ?//

/*
	-- Adds  a user to the room --
	* input : users username
	* output : None
*/
void Room::addUser(const string& username)
{
	_players.insert(username);
}


/*
	-- Removes a user from the room --
	* input : users username
	* output : None
*/
void Room::removeUser(const string& username)
{
	_players.erase(username);
}

/*
	-- Returns a set of all the users in the room -- 
	* input : None
	* output : Set of all the users
*/
std::set<string> Room::getAllUsers() const
{
	return _players;
}

/*
	-- Gets the state of a room --
	* input : None
	* output : room state
*/
std::string Room::isActive() const
{
	return _roomData._state;
}

/*
	-- Gets the data of a room --
	* input : None
	* output : rooms data
*/
RoomData Room::getData() const
{
	return _roomData;
}

void Room::activate()
{
	_roomData._state = "playing";
}

void Room::close()
{
	_roomData._state = "closed";
}
