#pragma once

#include "jsonUtils.h"
#include "LoginRequests.h"
#include "MenuRequests.h"
#include "StatisticsRequests.h"
#include "RoomRequests.h"
#include "GlobalRequests.h"
#include <vector>

using namespace JsonUtils;


class JsonRequestPacketDeserializer
{

public:

	// Methods for singleton
	JsonRequestPacketDeserializer(JsonRequestPacketDeserializer const&) = delete;
	void operator=(JsonRequestPacketDeserializer const&) = delete;

	static JsonRequestPacketDeserializer& getInstance()
	{
		static JsonRequestPacketDeserializer instance;

		return instance;
	}


	// Methods
	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);
	
	//	Menu 
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const Buffer& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer& buffer);
	static GetPersonalStatsRequest deserializeGetPersonalStats(const Buffer& buffer);
	static LogoutRequest deserializeLogoutReq(const Buffer& buffer);
	
	//	Room 
	static LeaveRoomRequest deserializeLeaveRoomReq(const Buffer& buffer);
	static CloseRoomRequest deserializeCloseRoomReq(const Buffer& buffer);
	static StartRoomRequest deserializeStartRoomReq(const Buffer& buffer);
	static RoomStateRequest deserializeRoomStateReq(const Buffer& buffer);


private:


	// Constructors
	JsonRequestPacketDeserializer() = default;
	
	// private Methods
	static void toJson(const Buffer& buffer, json& jsonContainer);
	static std::string returnField(const std::string field ,  const Buffer& buffer);
};

