#include "MenuRequestHandler.h"
#include "irreleventRequestException.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

typedef RequestResult(MenuRequestHandler::* PTR) (const Buffer& buffer);

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory* handleFactory) :
    _handleFactory(handleFactory)
{
}

/*
    -- find if the given request is relevent -- 
    * input: requestInfo
    * output: relevent or not
*/
bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    for (auto& requestID : { REQUEST_ID::CREATE_ROOM , REQUEST_ID::JOIN_ROOM ,
                             REQUEST_ID::GET_PLAYERS_IN_ROOM , REQUEST_ID::GET_HIGH_SCORE,
                             REQUEST_ID::GET_PLAYER_STATISTICS, REQUEST_ID::LOGOUT,
                             REQUEST_ID::GET_ROOMS })
    {
        if (requestID == requestInfo._id) 
            return true;
    }
    
    return false;
}


/* 
    -- handle request -- 
    * input: request info
    * output: request result
*/
RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    try {
        if (!isRequestRelevant(requestInfo))
            throw IrreleventRequestException();

        PTR handlers[] = {
            &MenuRequestHandler::createRoom,
            &MenuRequestHandler::joinRoom,
            &MenuRequestHandler::getPlayersInRoom,
            &MenuRequestHandler::getHighScore,
            &MenuRequestHandler::getStatistics,
            &MenuRequestHandler::logout,
            &MenuRequestHandler::getRooms,
            &MenuRequestHandler::createRoom,
        };


        int i = 0;

        for (auto& requestID : { REQUEST_ID::CREATE_ROOM , REQUEST_ID::JOIN_ROOM ,
                                 REQUEST_ID::GET_PLAYERS_IN_ROOM , REQUEST_ID::GET_HIGH_SCORE,
                                 REQUEST_ID::GET_PLAYER_STATISTICS, REQUEST_ID::LOGOUT,
                                 REQUEST_ID::GET_ROOMS })
        {
            if (requestID == requestInfo._id)
                return (this->*handlers[i])(requestInfo._buffer);
          
            ++i;
        }

    } catch (std::exception& e) {
        return {
            JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() }),
            _handleFactory->createMenuRequestHandler()
        };
    }
    return {};
}

RequestResult MenuRequestHandler::createRoom(const Buffer& buffer)
{
    CreateRoomRequest req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(buffer);

    _handleFactory->getRoomManager()->createRoom(req._roomData);

    return  { JsonResponsePacketSerializer::serializeResponse(OkResponse{}), _handleFactory->createRoomAdminRequestHandler() };

}

RequestResult MenuRequestHandler::getRooms(const Buffer& buffer)
{

    return { 
        JsonResponsePacketSerializer::serializeResponse({_handleFactory->getRoomManager()->GetRooms()}) , 
        _handleFactory->createMenuRequestHandler()
    };
}

RequestResult MenuRequestHandler::getPlayersInRoom(const Buffer& buffer)
{

    return {
        
        JsonResponsePacketSerializer::serializeResponse({
            _handleFactory->getRoomManager()->getPlayersInRoom(JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(buffer)._roomName)
        }),         
        _handleFactory->createMenuRequestHandler()

    };
}

RequestResult MenuRequestHandler::joinRoom(const Buffer& buffer)
{
    _handleFactory->getRoomManager()->joinRoom(JsonRequestPacketDeserializer::deserializeJoinRoomRequest(buffer));

    return { JsonResponsePacketSerializer::serializeResponse(OkResponse{}), _handleFactory->createRoomMemberRequestHandler() };
}

RequestResult MenuRequestHandler::getStatistics(const Buffer& buffer)
{
 
    return {
         JsonResponsePacketSerializer::serializeResponse({
            _handleFactory->getStatisticsManager()->getUserStatistics(JsonRequestPacketDeserializer::deserializeGetPersonalStats(buffer)._user
         )}),
        _handleFactory->createMenuRequestHandler()
    
    };

}

RequestResult MenuRequestHandler::getHighScore(const Buffer& buffer)
{
    return {

      JsonResponsePacketSerializer::serializeResponse({_handleFactory->getStatisticsManager()->getTopScores()}),
      _handleFactory->createMenuRequestHandler()

    };
}

RequestResult MenuRequestHandler::logout(const Buffer& buffer)
{
    _handleFactory->getLoginManager()->logout(JsonRequestPacketDeserializer::deserializeLogoutReq(buffer)._user);
    
    return  { 
        JsonResponsePacketSerializer::serializeResponse(OkResponse{}), _handleFactory->createLoginRequestHandler() 
    };
}
