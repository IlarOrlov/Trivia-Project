#pragma once

#include <string>
#include <set>
#include "RoomData.h"


class Room
{

public:
	
	// Constructor 
	Room() = default;
	Room(const RoomData& roomData);
	
	// Methods
	void addUser(const std::string& username);
	void removeUser(const std::string& username);
	std::set<std::string> getAllUsers() const;
	
	// getters
	std::string isActive() const;
	RoomData getData() const;

	// setters
	void activate();
	void close();

private:

	// Fields
	RoomData _roomData;
	std::set<std::string> _players;
	
};

