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

	NST::SerializeResult WriteBaseline( const size_t uiTableIndex, const size_t uiStringIndex, const float flTime, CNetworkBuffer& buffer );

private:
	CServer* const m_pServer;
};

#endif //CSERVERNETWORKSTRINGTABLEMANAGER_H