#ifndef CCLIENTNETWORKSTRINGTABLEMANAGER_H
#define CCLIENTNETWORKSTRINGTABLEMANAGER_H

#include "networking/stringtable/CNetworkStringTableManager.h"

#include "messages/sv_cl_messages/NetTable.pb.h"

class IGameClientInterface;

class CClientNetworkStringTableManager final : public CNetworkStringTableManager
{
public:

	void SetGameClient( IGameClientInterface* const pGameClient );

	void ProcessNetTableMessage( sv_cl_messages::NetTable& table );

private:
	IGameClientInterface* m_pGameClient = nullptr;
};

#endif //CCLIENTNETWORKSTRINGTABLEMANAGER_H