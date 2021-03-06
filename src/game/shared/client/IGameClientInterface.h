#ifndef GAME_CLIENT_IGAMECLIENTINTERFACE_H
#define GAME_CLIENT_IGAMECLIENTINTERFACE_H

#include "game/shared/IBaseGameInterface.h"

class INetworkStringTableManager;

class CCommand;

/**
*	@defgroup GameClient Game Client systems
*
*	@{
*/

/**
*	Client game library main interface.
*/
class IGameClientInterface : public IBaseGameInterface
{
public:

	/**
	*	Called when the client has connected to a server.
	*	TODO: should include a parameter that describes the server's basic info.
	*	@return true if the client should continue connecting, false otherwise.
	*/
	virtual bool ClientConnected() = 0;

	/**
	*	Called when a network string table has been created.
	*	@param pszName Name of the table that was created.
	*	@param manager Table manager.
	*/
	virtual void OnNetworkStringTableCreated( const char* const pszName, INetworkStringTableManager& manager ) = 0;

	/**
	*	Called when the client has fully connected to the server.
	*	TODO: should include a parameter that describes the server's basic info.
	*	TODO: needs an object that represents the client.
	*/
	virtual void ClientPutInServer() = 0;

	/**
	*	Called when the client has disconnected from a server.
	*	TODO: should include a parameter that describes the server's basic info.
	*	@param bWasFullyConnected Whether the client was fully connected to the server, or still in the process of connecting.
	*/
	virtual void ClientDisconnected( const bool bWasFullyConnected ) = 0;

	/**
	*	Called when the user has entered a command.
	*	@param command Object that contains command arguments.
	*	@return true if handled, false otherwise.
	*/
	virtual bool ClientCommand( const CCommand& command ) = 0;
};

/**
*	Game client interface name.
*/
#define IGAMECLIENTINTERFACE_NAME "IGAMECLIENTINTERFACEV001"

/** @} */

#endif //GAME_CLIENT_IGAMECLIENTINTERFACE_H