#include "RoomAdminRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "irreleventRequestException.h"


typedef RequestResult(RoomAdminRequestHandler::* PTR) (const Buffer& buffer);


RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory* handleFactory):
    _handleFactory(handleFactory)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    for (auto& requestID : { REQUEST_ID::CLOSE_ROOM , REQUEST_ID::START_ROOM,
                            REQUEST_ID::GET_ROOM_STATE })
    {
        if (requestID == requestInfo._id)
            return true;
    }

    return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{

    try {
        if (!isRequestRelevant(requestInfo))
            throw IrreleventRequestException();

        PTR handlers[] = {
            &RoomAdminRequestHandler::closeRoom,
            &RoomAdminRequestHandler::startRoom,
            &RoomAdminRequestHandler::getRoomState,
        };


        int i = 0;

        for (auto& requestID : { REQUEST_ID::CLOSE_ROOM , REQUEST_ID::START_ROOM,
                            REQUEST_ID::GET_ROOM_STATE })
        {
            if (requestID == requestInfo._id)
                return (this->*handlers[i])(requestInfo._buffer);

            ++i;
        }

    }
    catch (std::exception& e) {
        return {
            JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() }),
            _handleFactory->createMenuRequestHandler()
        };
    }
    return {};
}

RequestResult RoomAdminRequestHandler::closeRoom(const Buffer& buffer)
{
    _handleFactory->getRoomManager()->closeRoom(JsonRequestPacketDeserializer::deserializeCloseRoomReq(buffer)._roomName);
    return  { JsonResponsePacketSerializer::serializeResponse(OkResponse{}), _handleFactory->createMenuRequestHandler() };
}

RequestResult RoomAdminRequestHandler::startRoom(const Buffer& buffer)
{
    _handleFactory->getRoomManager()->activateRoom(JsonRequestPacketDeserializer::deserializeStartRoomReq(buffer)._roomName);
    return  { JsonResponsePacketSerializer::serializeResponse(OkResponse{}), _handleFactory->createRoomAdminRequestHandler() };
}

RequestResult RoomAdminRequestHandler::getRoomState(const Buffer& buffer)
{

    std::string roomName = JsonRequestPacketDeserializer::deserializeRoomStateReq(buffer)._roomName;
    return  {

        JsonResponsePacketSerializer::serializeResponse({
            _handleFactory->getRoomManager()->getRoomData(roomName),
            _handleFactory->getRoomManager()->getPlayersInRoom(roomName)
            }
        ),
        _handleFactory->createRoomAdminRequestHandler() 
    
    };
}
