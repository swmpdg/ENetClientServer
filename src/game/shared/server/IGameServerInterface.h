#ifndef GAME_SERVER_IGAMESERVERINTERFACE_H
#define GAME_SERVER_IGAMESERVERINTERFACE_H

#include "game/shared/IBaseGameInterface.h"

class CNetworkStringTableManager;

class CCommand;

/**
*	Server game library main interface.
*/
class IGameServerInterface : public IBaseGameInterface
{
public:

	/**
	*	Called when all network string tables should be created.
	*/
	virtual void CreateNetworkStringTables( CNetworkStringTableManager& manager ) = 0;

	/**
	*	Called when a client connects.
	*	TODO: needs an object that represents the client.
	*	@param pszRejectReason Reason why the client is being rejected, if any.
	*	@param uiRejectSizeInBytes Size of the pszRejectReason buffer, in bytes.
	*	@return true to allow them to continue joining, or false to reject them. Enter a reject reason in pszRejectReason to give the client a reason.
	*/
	virtual bool ClientConnect( char* pszRejectReason, const size_t uiRejectSizeInBytes ) = 0;

	/**
	*	Called when a client has finished connecting and is placed in the world.
	*	TODO: needs an object that represents the client.
	*/
	virtual void ClientPutInServer() = 0;

	/**
	*	Called when a client has disconnected.
	*	TODO: needs an object that represents the client.
	*	@param bWasFullyConnected If true, the client was fully connected, meaning they've gone through ClientPutInServer. Otherwise, is false.
	*/
	virtual void ClientDisconnected( const bool bWasFullyConnected ) = 0;

	/**
	*	Called when a client has entered a command.
	*	TODO: needs an object that represents the client.
	*	@param command Object that contains command arguments.
	*	@return true if handled, false otherwise.
	*/
	virtual bool ClientCommand( const CCommand& command ) = 0;
};

#define IGAMESERVERINTERFACE_NAME "IGAMESERVERINTERFACEV001"

#endif //GAME_SERVER_IGAMESERVERINTERFACE_H