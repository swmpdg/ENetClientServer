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
}

void CGameClient::ClientDisconnected( const bool bWasFullyConnected )
{
	printf( "client: Client disconnected\n" );
}

bool CGameClient::ClientCommand( const CCommand& command )
{
	assert( m_pClientTable );

	if( strcmp( command.Arg( 0 ), "liststrings" ) == 0 )
	{
		printf( "Number of strings in table %s: %u\n", m_pClientTable->GetName(), m_pClientTable->GetStringCount() );

		for( size_t uiIndex = 0; uiIndex < m_pClientTable->GetStringCount(); ++uiIndex )
		{
			printf( "String %u: %s\n", uiIndex, m_pClientTable->GetString( uiIndex ) );
		}

		return true;
	}

	return false;
}