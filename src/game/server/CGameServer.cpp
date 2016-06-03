#include "networking/stringtable/CNetworkStringTableManager.h"
#include "networking/stringtable/CNetworkStringTable.h"

#include "utility/CCommand.h"

#include "CGameServer.h"

REGISTER_SINGLE_INTERFACE( IGAMESERVERINTERFACE_NAME, CGameServer );

void CGameServer::CreateNetworkStringTables( CNetworkStringTableManager& manager )
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
	if( strcmp( command.Arg( 0 ), "addstring" ) == 0 )
	{
		char szBuffer[ 256 ];

		for( size_t uiIndex = 0; uiIndex < 100; ++uiIndex )
		{
			snprintf( szBuffer, sizeof( szBuffer ), "foo %u", m_uiStringOffset++ );

			m_pServerTable->Add( szBuffer );
		}
	}

	return false;
}