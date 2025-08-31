
#include "JsonRequestPacketDeserializer.h"
#include <bitset>
#include <sstream>
#include "JsonResponsePacketSerializer.h"
#include "MyException.h"

using std::string;
using std::bitset;


/*
    -- deserialize login packet from bits to LoginRequest message --
    * input: vector of chars (bits) formated as JSON
    * output: SignupRequest message with the bits data
*/
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(
    const Buffer& buffer)
{
   
    json jsonContainer;
    toJson(buffer, jsonContainer);

    return LoginRequest { jsonContainer["username"].get<string>(),
                          jsonContainer["password"].get<string>() };

}

/*
    -- deserialize signup packet from bits to SignupRequest message --
    * input: vector of chars (bits) formated as JSON
    * output: SignupRequest message with the bits data
*/
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(
    const Buffer& buffer)
{

    json jsonContainer;
    toJson(buffer, jsonContainer);

    return  { 
        jsonContainer["username"].get<string>(),
        jsonContainer["password"].get<string>(),
        jsonContainer["email"].get<string>(),
        jsonContainer["phone"].get<string>(),
        jsonContainer["date"].get<string>(),
        {
            jsonContainer["street"].get<string>(),
            jsonContainer["apartment"].get<string>(),
            jsonContainer["city"].get<string>()
        }
    };

}

/*
    -- deserialize get players in room  packet from bits to GetPlayersInRoomRequest message --
    * input: vector of chars (bits) formated as JSON
    * output: GetPlayersInRoomRequest message with the bits data
*/
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(
    const Buffer& buffer)
{

    return { returnField("room_name" , buffer) };
}

/*
    -- deserialize join room  packet from bits to JoinRoomRequest message --
    * input: vector of chars (bits) formated as JSON
    * output: JoinRoomRequest message with the bits data
*/
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer)
{
    
    json jsonContainer;
    toJson(buffer, jsonContainer);

    return JoinRoomRequest {

         jsonContainer["room_name"].get<string>(),
         jsonContainer["username"].get<string>()
   
    };
}

/*
    -- deserialize create room  packet from bits to CreateRoomRequest message --
    * input: vector of chars (bits) formated as JSON
    * output: CreateRoomRequest message with the bits data
*/
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer)
{

    json jsonContainer;
    toJson(buffer, jsonContainer);
        
    return 
    {
        {
            jsonContainer["creator"].get<string>(), 
            jsonContainer["room_name"].get<string>(),
            "waiting",
            jsonContainer["max_users"].get<int>(),
            jsonContainer["questions_count"].get<int>(),
            jsonContainer["answer_timeout"].get<int>()
        }
    };
}

GetPersonalStatsRequest JsonRequestPacketDeserializer::deserializeGetPersonalStats(const Buffer& buffer)
{
    return {
        returnField("username" , buffer)
    };
}



LogoutRequest JsonRequestPacketDeserializer::deserializeLogoutReq(const Buffer& buffer)
{
    return {
        returnField("username" , buffer)
    };
}

LeaveRoomRequest JsonRequestPacketDeserializer::deserializeLeaveRoomReq(const Buffer& buffer)
{

    json jsonContainer;
    toJson(buffer, jsonContainer);

    return {
         jsonContainer["username"].get<string>(),
         jsonContainer["room_name"].get<string>()
    };
}

CloseRoomRequest JsonRequestPacketDeserializer::deserializeCloseRoomReq(const Buffer& buffer)
{
    return {
         returnField("room_name" , buffer)
    };
}

StartRoomRequest JsonRequestPacketDeserializer::deserializeStartRoomReq(const Buffer& buffer)
{
    return {
        returnField("room_name" , buffer)
    };
}


RoomStateRequest JsonRequestPacketDeserializer::deserializeRoomStateReq(const Buffer& buffer)
{
    return {
        returnField("room_name" , buffer)
    };
}


/*
    -- convert the bits buffer to json format --
    * input: json in bytes buffer, json container to contain the result 
*/
void JsonRequestPacketDeserializer::toJson(const Buffer& buffer,
    json& jsonContainer)
{

    string bufferString = "";
    // setting the buffer as a stream
    for (auto& bit : buffer)
        bufferString += bit;


    std::stringstream buffstream(bufferString);
    bitset<BYTE_SIZE> byte;
    string jsonStringHolder;

    // loop throw all bits
    while (buffstream.good())
    {

        buffstream >> byte;
        jsonStringHolder += char(byte.to_ulong());

    }


    jsonContainer = json::parse(jsonStringHolder);

}

string JsonRequestPacketDeserializer::returnField(const string field, const Buffer& buffer)
{
    json jsonContainer;
    toJson(buffer, jsonContainer);

    return jsonContainer[field].get<string>();
}
