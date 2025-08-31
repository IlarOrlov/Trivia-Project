#include "JsonResponsePacketSerializer.h"
#include <bitset>
#include <sstream>
#include <iostream>
#include "Room.h"
#include "MyException.h"

using std::bitset;
using std::string;
using std::stringstream;




//? OK & ERROR RESPONSES ?//

/* -- serializing an login message in Json format -- */
Buffer JsonResponsePacketSerializer::serializeResponse(const OkResponse& response)
{
	Buffer buffer;

	
	// appending to the the buffer all the bytes
	for (auto& bit : response._msg )
		buffer.push_back(bit);

	return buffer;

}

/* -- serializing an error message in Json format -- */
Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& response)
{
	return formatMessage( "{ \"error\" :  \"" + response._error + "\" }" );
}



//? ROOM RESPONSES ?//




/* -- serializing an GetRoom message in Json format -- */
Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
	string msg = "{ \"rooms\" : [ ";	
	
	stringstream helper;

	if (response._rooms.empty())
		return formatMessage("");
	
	
	for (auto& room : response._rooms)
		helper  << "{" << makeJsonPair("name", room.first) << ", "
				<< makeJsonPair("admin", room.second.getData()._admin) << ", "
				<< makeJsonPair("state", room.second.isActive()) << ", "
				<< makeJsonPair("players_count", room.second.getAllUsers().size()) << ", "
				<< makeJsonPair("max_players", room.second.getData()._maxPlayers) << ", "
				<< makeJsonPair("questions_count", room.second.getData()._questionsCount) << ", "
				<< makeJsonPair("question_timeout", room.second.getData()._questionTimeout) << " }, ";
	
	msg += helper.str();
	
	return formatMessage(msg.substr(0, msg.size() - 2) + " ] }");

}


/* -- serializing an GetRoomState message in Json format -- */
Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateRespone& response)
{
	string msg = "{ " + makeJsonPair("state", response._roomData._state) + " , \"players\" : [ ";

	for (auto& player : response._users)
		msg += "\"" + player + "\" ,";


	// getting rid of the last ','
	msg.pop_back();


	// closing array of players
	msg += "], " 
		+  makeJsonPair("answer_count", response._roomData._questionsCount) + "," 
		+  makeJsonPair("answer_timeout", response._roomData._questionTimeout) + ","
		+  makeJsonPair("admin", response._roomData._admin) + "}";

	return formatMessage(msg);
}


/* -- serializing an GetPlayerInRoom message in Json format -- */
Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
	string msg = "{ \"players\" : [ ";

	for (auto& player : response._players)
		msg += "\"" + player +  "\"" + ", ";

	return formatMessage(msg.substr(0, msg.size() - 2) + " ] }");
}



//? STATISTICS RESPONSES ?//

/* -- serializing an getHighScores message in Json format -- */
Buffer JsonResponsePacketSerializer::serializeResponse(const getHighScoreResponse& response)
{

	string msg = "{ \"high_scores\" : { ";

	for (auto stat : response._statistics)
		msg += makeJsonPair(stat.second, stat.first) + ", ";

	return formatMessage(msg.substr(0, msg.size() - 2) + " } }");

}

/* -- serializing an getPersonalStats message in Json format -- */
Buffer JsonResponsePacketSerializer::serializeResponse(const getPersonalStatsResponse& res)
{
	return formatMessage(

		string("{ \"personal_stats\" : { ") +

		makeJsonPair("time_for_answer", res._statistics._timeForAnswer) + ", " +
		makeJsonPair("right_answers", res._statistics._rightAnswers) + ", " +
		makeJsonPair("wrong_answers", res._statistics._wrongAnswers) + ", " +
		makeJsonPair("games_played", res._statistics._gamesPlayed) + " } }");
}




//? PRIVATE METHODS ?//


Buffer JsonResponsePacketSerializer::formatMessage(const string& data)
{
	Buffer buffer;

	// setting the code to 0
	for (auto& bit : CODE)
		buffer.push_back(bit);


	// getting the length of data in bytes
	int dataLength = data.size() * BYTE_SIZE;


	// adding the len of data to the buffer
	for (auto& bit : bitset<DATA_LENGTH>(dataLength).to_string())
		buffer.push_back(bit);


	// adding the data to the buffer
	for (auto& letter : data)
		for (auto& bit : bitset<BYTE_SIZE>(letter).to_string())
			buffer.push_back(bit);

	return buffer;
}


