#pragma once

#include <string>

struct RoomData
{

	std::string _admin,
				_name,
				_state;
	
	/* 
	~states:
		waiting = waiting on admin to start
		playing = admin and players are currently playing
	*/

	int _maxPlayers,
		_questionsCount,
		_questionTimeout;
};