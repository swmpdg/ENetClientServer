#include <cassert>

#include "utility/Util.h"

#include "networking/CNetworkBuffer.h"

#include "networking/stringtable/CNetworkStringTable.h"

#include "game/shared/client/IGameClientInterface.h"

#include "messages/sv_cl_messages/NetTables.pb.h"
#include "messages/sv_cl_messages/NetTable.pb.h"

#include "CClientNetworkStringTableManager.h"

void CClientNetworkStringTableManager::SetGameClient( IGameClientInterface* const pGameClient )
{
	m_pGameClient = pGameClient;
}

void CClientNetworkStringTableManager::ProcessNetTablesMessage( const sv_cl_messages::NetTables& tables )
{
	for( int iIndex = 0; iIndex < tables.tables_size(); ++iIndex )
	{
		const auto table = tables.tables( iIndex );

		SetAllowTableCreation( true );

		auto pTable = CreateTable( g_StringPool.Allocate( table.name().c_str() ), table.maxentries() );

		SetAllowTableCreation( false );

		if( pTable )
		{
			m_pGameClient->OnNetworkStringTableCreated( pTable->GetName(), *this );
		}
		else
		{
			printf( "Failed to create client network string table %s!\n", table.name().c_str() );
		}
	}
}

void CClientNetworkStringTableManager::ProcessNetTableMessage( sv_cl_messages::NetTable& table )
{
	assert( m_pGameClient );

	switch( table.command() )
	{
	case sv_cl_messages::NetTable_Command_CLEAR:
		{
			assert( sv_cl_messages::NetTable::kTableID == table.oneof_tableID_case() );

			//TODO

			break;
		}

	case sv_cl_messages::NetTable_Command_UPDATE:
		{
			assert( sv_cl_messages::NetTable::kTableID == table.oneof_tableID_case() );

			assert( table.has_data() );

			auto& data = *table.mutable_data();

			CNetworkBuffer buffer( "CClientNetworkStringTableManager::ProcessNetTableMessage_update_buffer", reinterpret_cast<uint8_t*>( const_cast<char*>( data.data() ) ), data.length() );

			UnserializeTable( table.tableid(), buffer );

			break;
		}
	}
}