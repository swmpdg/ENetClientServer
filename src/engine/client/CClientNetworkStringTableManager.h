#ifndef CCLIENTNETWORKSTRINGTABLEMANAGER_H
#define CCLIENTNETWORKSTRINGTABLEMANAGER_H

#include "networking/stringtable/CNetworkStringTableManager.h"

class IGameClientInterface;

namespace sv_cl_messages
{
class NetTables;
class NetTable;
}

class CClientNetworkStringTableManager final : public CNetworkStringTableManager
{
public:

	void SetGameClient( IGameClientInterface* const pGameClient );

	void ProcessNetTablesMessage( const sv_cl_messages::NetTables& tables );

	void ProcessNetTableMessage( sv_cl_messages::NetTable& table );

private:
	IGameClientInterface* m_pGameClient = nullptr;
};

#endif //CCLIENTNETWORKSTRINGTABLEMANAGER_H