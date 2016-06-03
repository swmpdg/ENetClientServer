#include <cstdlib>
#include <cstdio>

#include "networking/shared/stringtable/INetworkStringTableManager.h"
#include "networking/shared/stringtable/INetworkStringTable.h"

#include "utility/CCommand.h"

#include "CGameServer.h"

REGISTER_SINGLE_INTERFACE( IGAMESERVERINTERFACE_NAME, CGameServer );

void CGameServer::CreateNetworkStringTables( INetworkStringTableManager& manager )
{
	m_pServerTable = manager.CreateTable( "table" );
}

bool CGameServer::ClientConnect( char* pszRejectReason, const size_t uiRejectSizeInBytes )
{
	return true;
}

void CGameServer::ClientPutInServer()
{
	//TODO: set up player settings, spawn player, etc.
}

void CGameServer::ClientDisconnected( const bool bWasFullyConnected )
{
	//TODO: clear any non-persistent player data.
}

bool CGameServer::ClientCommand( const CCommand& command )
{
	if( strcmp( command.Arg( 0 ), "addstrings" ) == 0 )
	{
		char szBuffer[ 256 ];

		const char* const pszName = command.ArgC() > 1 ? command.Arg( 1 ) : "foo";

		const size_t uiCount = command.ArgC() > 2 ? strtoul( command.Arg( 2 ), nullptr, 10 ) : 100;

		for( size_t uiIndex = 0; uiIndex < uiCount; ++uiIndex )
		{
			snprintf( szBuffer, sizeof( szBuffer ), "%s %u", pszName, m_uiStringOffset++ );

			m_pServerTable->Add( szBuffer );
		}

		return true;
	}

	return false;
}