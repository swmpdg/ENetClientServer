#include "CServer.h"

#include "messages/sv_cl_messages/NetTable.pb.h"

#include "NetworkConstants.h"

#include "utility/CNetworkStringTable.h"
#include "utility/NetworkUtils.h"

#include "CSVClient.h"

#include "CServerNetworkStringTableManager.h"

CServerNetworkStringTableManager::CServerNetworkStringTableManager( CServer* const pServer )
	: m_pServer( pServer )
{
}

CNetworkStringTable* CServerNetworkStringTableManager::CreateTable( const char* const pszName )
{
	if( !pszName || !( *pszName ) )
		return nullptr;

	if( GetTableByName( pszName ) )
		return nullptr;

	auto pTable = new CNetworkStringTable( pszName, m_Tables.size() );

	uint8_t bufData[ MAX_DATAGRAM ];

	CNetworkBuffer buffer( "CServerNetworkStringTableManager::CreateTable_Buffer", bufData, sizeof( bufData ) );

	WriteNetTableCreateMessage( pTable, buffer );

	if( !buffer.HasOverflowed() )
	{
		m_Tables.push_back( pTable );

		CSVClient* pClients = m_pServer->GetClients();

		for( size_t uiIndex = 0; uiIndex < m_pServer->GetMaxClients(); ++uiIndex, ++pClients )
		{
			if( pClients->IsFullyConnected() )
			{
				pClients->SendMessage( buffer );
			}
		}

		return pTable;
	}
	else
	{
		delete pTable;

		printf( "CServerNetworkStringTableManager::CreateTable: Couldn't write table creation message for table %s!\n", pszName );

		return nullptr;
	}
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