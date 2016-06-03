#include "CServer.h"

#include "messages/sv_cl_messages/NetTables.pb.h"

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
	sv_cl_messages::NetTables tables;

	for( auto pTable : m_Tables )
	{
		auto pTableField = tables.add_tables();

		pTableField->set_name( pTable->GetName() );
	}


	SerializeToBuffer( SVCLMessage::NETTABLES, tables, buffer );
}

bool CServerNetworkStringTableManager::WriteBaseline( const size_t uiTableIndex, const size_t uiStringIndex, const float flTime, CNetworkBuffer& buffer )
{
	auto pTable = GetTableByIndex( uiTableIndex );

	if( !pTable )
		return false;

	return pTable->Serialize( buffer, flTime, uiStringIndex );
}