#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

using std::shared_ptr;


//? Constructors ?// 


/* -- C'tor -- */
RequestHandlerFactory::RequestHandlerFactory(shared_ptr<IDatabase>& database) :
    _database(database), _loginManager(&LoginManager::getInstance(database)) , _roomManager(new RoomManager),
    _statisticsManager(&StatisticsManager::getInstance(database))
 
{
}


//? Methods ?//


/* -- create new login loginRequestHandler -- */
shared_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{

    // make new shared_ptr to LoginRequestHandler
    return shared_ptr<IRequestHandler>(
            new LoginRequestHandler(RequestHandlerFactory::_loginManager,
                                    this));

}

/* -- create new MenuRequestHandler -- */
shared_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler()
{

    // make new shared_ptr to MenuRequestHandler
    return shared_ptr<IRequestHandler>(new MenuRequestHandler(this));

}

std::shared_ptr<IRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler()
{
    return shared_ptr<IRequestHandler>(new RoomAdminRequestHandler(this));
}

std::shared_ptr<IRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler()
{
    return shared_ptr<IRequestHandler>(new RoomMemberRequestHandler(this));
}

