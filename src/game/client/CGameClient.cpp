#include <cassert>
#include <cstdio>
#include <cstring>

#include "utility/CCommand.h"

#include "networking/shared/stringtable/INetworkStringTableManager.h"
#include "networking/shared/stringtable/INetworkStringTable.h"

#include "CGameClient.h"

REGISTER_SINGLE_INTERFACE( IGAMECLIENTINTERFACE_NAME, CGameClient );

bool CGameClient::ClientConnected()
{
	return true;
}

void CGameClient::OnNetworkStringTableCreated( const char* const pszName, INetworkStringTableManager& manager )
{
	if( strcmp( pszName, "table" ) == 0 )
	{
		m_pClientTable = manager.GetTableByName( pszName );
	}
	else if( strcmp( pszName, "table2" ) == 0 )
	{
		m_pClientTable2 = manager.GetTableByName( pszName );
	}
}

void CGameClient::ClientPutInServer()
{
}

void CGameClient::ClientDisconnected( const bool bWasFullyConnected )
{
	printf( "client: Client disconnected\n" );
}

static void ListStrings( INetworkStringTable* pTable )
{
	printf( "Number of strings in table %s: %u\n", pTable->GetName(), pTable->GetStringCount() );

	for( size_t uiIndex = 0; uiIndex < pTable->GetStringCount(); ++uiIndex )
	{
		printf( "String %u: %s\n", uiIndex, pTable->GetString( uiIndex ) );
	}
}

bool CGameClient::ClientCommand( const CCommand& command )
{
	assert( m_pClientTable );

	if( strcmp( command.Arg( 0 ), "liststrings" ) == 0 )
	{
		ListStrings( m_pClientTable );

		return true;
	}
	else if( strcmp( command.Arg( 0 ), "liststrings2" ) == 0 )
	{
		ListStrings( m_pClientTable2 );

		return true;
	}

	return false;
}