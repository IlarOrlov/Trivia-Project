#include "RoomMemberRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "irreleventRequestException.h"


typedef RequestResult(RoomMemberRequestHandler::* PTR) (const Buffer& buffer);



RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory* handleFactory):
    _handleFactory(handleFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    for (auto& requestID : { REQUEST_ID::LEAVE_ROOM, REQUEST_ID::GET_ROOM_STATE })
        if (requestID == requestInfo._id)
            return true;

    return false;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{

    try {
        if (!isRequestRelevant(requestInfo))
            throw IrreleventRequestException();

        PTR handlers[] = {
            &RoomMemberRequestHandler::leaveRoom,
            &RoomMemberRequestHandler::getRoomState,
        };


        int i = 0;

        for (auto& requestID : { REQUEST_ID::LEAVE_ROOM, REQUEST_ID::GET_ROOM_STATE })
        {
            if (requestID == requestInfo._id)
                return (this->*handlers[i])(requestInfo._buffer);

            ++i;
        }

    }
    catch (std::exception& e) {
        return {
            JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() }),

        };
    }
    return {};
}

RequestResult RoomMemberRequestHandler::leaveRoom(const Buffer& buffer)
{
    _handleFactory->getRoomManager()->leaveRoom(JsonRequestPacketDeserializer::deserializeLeaveRoomReq(buffer));

    return  { 
        JsonResponsePacketSerializer::serializeResponse(OkResponse{}), 
        _handleFactory->createMenuRequestHandler() 
    };

}

RequestResult RoomMemberRequestHandler::getRoomState(const Buffer& buffer)
{
    auto roomName = JsonRequestPacketDeserializer::deserializeRoomStateReq(buffer)._roomName;
    auto roomData = _handleFactory->getRoomManager()->getRoomData(roomName);

    return  {

        JsonResponsePacketSerializer::serializeResponse({
            roomData,
            _handleFactory->getRoomManager()->getPlayersInRoom(roomName)
            }
        ),
        
        roomData._state == "closed" ?
        _handleFactory->createMenuRequestHandler()
        : _handleFactory->createRoomMemberRequestHandler()

    };
}
