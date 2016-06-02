#ifndef CCLIENTNETWORKSTRINGTABLEMANAGER_H
#define CCLIENTNETWORKSTRINGTABLEMANAGER_H

#include "utility/CNetworkStringTableManager.h"

#include "messages/sv_cl_messages/NetTable.pb.h"

class CClientNetworkStringTableManager final : public CNetworkStringTableManager
{
public:

	CNetworkStringTable* CreateTable( const char* const pszName ) override final;

	void ProcessNetTableMessage( sv_cl_messages::NetTable& table );
};

#endif //CCLIENTNETWORKSTRINGTABLEMANAGER_H