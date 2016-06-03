#include "CServer.h"

#include "messages/sv_cl_messages/NetTable.pb.h"

#include "networking/NetworkConstants.h"

#include "networking/stringtable/CNetworkStringTable.h"
#include "networking/NetworkUtils.h"

#include "CSVClient.h"

#include "CServerNetworkStringTableManager.h"

CServerNetworkStringTableManager::CServerNetworkStringTableManager( CServer* const pServer )
	: m_pServer( pServer )
{
}

void CServerNetworkStringTableManager::WriteNetTableCreateMessages( CNetworkBuffer& buffer )
{
	for( auto pTable : m_Tables )
	{
		WriteNetTableCreateMessage( pTable, buffer );
	}
}

void CServerNetworkStringTableManager::WriteNetTableCreateMessage( const CNetworkStringTable* const pTable, CNetworkBuffer& buffer )
{
	sv_cl_messages::NetTable table;

	table.set_command( sv_cl_messages::NetTable_Command_CREATE );

	table.set_name( pTable->GetName() );

	SerializeToBuffer( SVCLMessage::NETTABLE, table, buffer );
}