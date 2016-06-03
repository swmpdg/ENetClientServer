#ifndef GAME_SERVER_CGAMESERVER_H
#define GAME_SERVER_CGAMESERVER_H

#include "game/shared/server/IGameServerInterface.h"

class INetworkStringTable;

class CGameServer final : public IGameServerInterface
{
public:
	CGameServer() = default;

	void CreateNetworkStringTables( INetworkStringTableManager& manager ) override final;

	bool ClientConnect( char* pszRejectReason, const size_t uiRejectSizeInBytes ) override final;

	void ClientPutInServer() override final;

	void ClientDisconnected( const bool bWasFullyConnected ) override final;

	bool ClientCommand( const CCommand& command ) override final;

private:
	INetworkStringTable* m_pServerTable = nullptr;

	size_t m_uiStringOffset = 0;

private:
	CGameServer( const CGameServer& ) = delete;
	CGameServer& operator=( const CGameServer& ) = delete;
};

#endif //GAME_SERVER_CGAMESERVER_H