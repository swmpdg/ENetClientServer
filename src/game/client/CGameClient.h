#ifndef GAME_CLIENT_CGAMECLIENT_H
#define GAME_CLIENT_CGAMECLIENT_H

#include "game/shared/client/IGameClientInterface.h"

class INetworkStringTable;

/**
*	@ingroup GameClient
*
*	@{
*/

/**
*	Game client interface implementation.
*/
class CGameClient final : public IGameClientInterface
{
public:
	/**
	*	Constructor.
	*/
	CGameClient() = default;

	bool ClientConnected() override final;

	void OnNetworkStringTableCreated( const char* const pszName, INetworkStringTableManager& manager ) override final;

	void ClientPutInServer() override final;

	void ClientDisconnected( const bool bWasFullyConnected ) override final;

	bool ClientCommand( const CCommand& command ) override final;

private:
	INetworkStringTable* m_pClientTable = nullptr;
	INetworkStringTable* m_pClientTable2 = nullptr;

private:
	CGameClient( const CGameClient& ) = delete;
	CGameClient& operator=( const CGameClient& ) = delete;
};

/** @} */

#endif //GAME_CLIENT_CGAMECLIENT_H