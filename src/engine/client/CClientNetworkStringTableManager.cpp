#include <cassert>

#include "utility/Util.h"

#include "networking/CNetworkBuffer.h"

#include "networking/stringtable/CNetworkStringTable.h"

#include "CClientNetworkStringTableManager.h"

CNetworkStringTable* CClientNetworkStringTableManager::CreateTable( const char* const pszName )
{
	if( !pszName || !( *pszName ) )
		return nullptr;

	//Don't create, just return.
	return GetTableByName( pszName );
}

void CClientNetworkStringTableManager::ProcessNetTableMessage( sv_cl_messages::NetTable& table )
{
	switch( table.command() )
	{
	case sv_cl_messages::NetTable_Command_CREATE:
		{
			assert( sv_cl_messages::NetTable::kName == table.oneof_tableID_case() );

			m_Tables.push_back( new CNetworkStringTable( g_StringPool.Allocate( table.name().c_str() ), m_Tables.size() ) );

			break;
		}

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