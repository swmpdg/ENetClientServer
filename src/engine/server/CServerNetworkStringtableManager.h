#ifndef CSERVERNETWORKSTRINGTABLEMANAGER_H
#define CSERVERNETWORKSTRINGTABLEMANAGER_H

#include "networking/stringtable/CNetworkStringTableManager.h"

class CServer;

class CNetworkBuffer;

class CServerNetworkStringTableManager final : public CNetworkStringTableManager
{
public:
	CServerNetworkStringTableManager( CServer* const pServer );

	/**
	*	Write create messages for each table into the buffer.
	*/
	void WriteNetTableCreateMessages( CNetworkBuffer& buffer );

private:
	static void WriteNetTableCreateMessage( const CNetworkStringTable* const pTable, CNetworkBuffer& buffer );

private:
	CServer* const m_pServer;
};

#endif //CSERVERNETWORKSTRINGTABLEMANAGER_H