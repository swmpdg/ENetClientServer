#include <cassert>
#include <cstdio>
#include <memory>

#include <enet/enet.h>

#include "shared/Platform.h"

#include "networking/NetworkConstants.h"

#include "networking/CNetworkBuffer.h"
#include "networking/NetworkUtils.h"

#include "utility/CWorldTime.h"

#include "shared/Utility.h"

#include "game/shared/server/IGameServerInterface.h"

#include "CSVClient.h"

#include "CServer.h"

CServer::CServer()
	: m_NetStringTableManager( this )
{
}

CServer::~CServer()
{
	assert( !m_bInitialized );
	assert( !m_pHost );
}

bool CServer::Connect( const CreateInterfaceFn* factories, const size_t uiNumFactories )
{
	assert( factories );
	assert( uiNumFactories );

	for( size_t uiIndex = 0; uiIndex < uiNumFactories; ++uiIndex )
	{
		auto factory = factories[ uiIndex ];

		if( !m_pGameServer )
		{
			m_pGameServer = static_cast<IGameServerInterface*>( factory( IGAMESERVERINTERFACE_NAME, nullptr ) );
		}
	}

	return m_pGameServer != nullptr;
}

bool CServer::Initialize( const size_t uiMaxClients, const enet_uint16 uiPort )
{
	assert( !m_bInitialized );

	m_bInitialized = true;

	const ENetAddress address{ ENET_HOST_ANY, uiPort };

	m_pHost = enet_host_create( &address, uiMaxClients, NetChannel::COUNT, 0, 0 );

	if( !m_pHost )
		return false;

	m_pClients = new CSVClient[ uiMaxClients ];

	m_uiMaxClients = uiMaxClients;

	return true;
}

void CServer::ChangeLevel( const char* const pszMapName )
{
	assert( pszMapName);

	m_NetStringTableManager.Clear();

	//TODO: currently doesn't do anything map related. Just pretend it's working.

	m_NetStringTableManager.SetAllowTableCreation( true );

	m_pGameServer->CreateNetworkStringTables( m_NetStringTableManager );

	m_NetStringTableManager.SetAllowTableCreation( false );

}

void CServer::Shutdown()
{
	assert( m_bInitialized );

	m_bInitialized = false;

	m_NetStringTableManager.Clear();

	if( m_pClients )
	{
		for( size_t uiIndex = 0; uiIndex < m_uiMaxClients; ++uiIndex )
		{
			auto& client = m_pClients[ uiIndex ];

			if( client.IsConnected() )
			{
				enet_peer_reset( client.GetPeer() );
				client.Reset();
			}
		}

		delete[] m_pClients;
		m_pClients = nullptr;

		m_uiMaxClients = 0;
	}

	if( m_pHost )
	{
		enet_host_destroy( m_pHost );
		m_pHost = nullptr;
	}
}

void CServer::RunFrame()
{
	ProcessNetworkEvents();

	SendNetTables();

	DispatchClientMessages();
}

bool CServer::SendBroadcastMessage( const SVCLMessage messageId, google::protobuf::Message& message )
{
	uint8_t szBuffer[ MAX_DATAGRAM ];

	CNetworkBuffer buffer( "CServer::SendBroadcastMessage_Buffer", szBuffer, sizeof( szBuffer ) );

	if( !SerializeToBuffer( messageId, message, buffer ) )
		return false;

	bool bSuccess = true;

	for( size_t uiIndex = 0; uiIndex < m_uiMaxClients; ++uiIndex )
	{
		auto& client = m_pClients[ uiIndex ];

		if( client.IsFullyConnected() )
		{
			bSuccess = client.SendMessage( buffer ) && bSuccess;
		}
	}

	return bSuccess;
}

bool CServer::ClientCommand( const CCommand& command )
{
	if( m_pGameServer->ClientCommand( command ) )
		return true;

	return false;
}

void CServer::ProcessNetworkEvents()
{
	assert( m_bInitialized );
	assert( m_pHost );

	ENetEvent event;

	while( enet_host_service( m_pHost, &event, 0 ) > 0 )
	{
		switch( event.type )
		{
		case ENET_EVENT_TYPE_CONNECT:
			{
				//A client has connected.
				bool bAssignedSlot = false;

				auto disconnectReason = SVDisconnectCode::NO_FREE_SLOTS;

				for( size_t uiIndex = 0; uiIndex < m_uiMaxClients; ++uiIndex )
				{
					auto& client = m_pClients[ uiIndex ];

					if( !client.IsConnected() )
					{
						bAssignedSlot = true;

						//TODO: assign edict
						client.Initialize( event.peer );

						char szRejectReason[ MAX_BUFFER_LENGTH ];

						memset( szRejectReason, 0, sizeof( szRejectReason ) );

						if( m_pGameServer->ClientConnect( szRejectReason, sizeof( szRejectReason ) ) )
						{
							printf( "Client connected\n" );

							client.SendServerInfo( *this );
						}
						else
						{
							client.Disconnect( SVDisconnectCode::CONNECTION_REJECTED, szRejectReason );
						}

						break;
					}
				}

				if( !bAssignedSlot )
				{
					NET::DisconnectWithReason( event.peer, SVDisconnectCode::NO_FREE_SLOTS );
				}

				break;
			}

		case ENET_EVENT_TYPE_DISCONNECT:
			{
				if( event.peer->data )
				{
					CSVClient* const pClient = reinterpret_cast<CSVClient*>( event.peer->data );

					assert( ( pClient - m_pClients ) >= 0 && static_cast<size_t>( pClient - m_pClients ) < m_uiMaxClients );
					assert( pClient->IsConnected() || pClient->GetConnectionState() == SVClientConnState::PENDINGDISCONNECT );

					printf( "server: Client disconnected\n" );

					m_pGameServer->ClientDisconnected( pClient->IsFullyConnected() );

					pClient->Reset();
				}

				break;
			}

		case ENET_EVENT_TYPE_RECEIVE:
			{
				ProcessPacket( *reinterpret_cast<CSVClient*>( event.peer->data ), event.packet );

				enet_packet_destroy( event.packet );
				break;
			}
		}
	}
}

void CServer::ProcessPacket( CSVClient& client, ENetPacket* pPacket )
{
	assert( pPacket );

	CNetworkBuffer buffer{ "SV_Packet", pPacket->data, pPacket->dataLength };

	client.ProcessMessages( *this, buffer );
}

void CServer::SendNetTables()
{
	for( size_t uiIndex = 0; uiIndex < m_uiMaxClients; ++uiIndex )
	{
		auto& client = m_pClients[ uiIndex ];

		if( client.IsFullyConnected() )
		{
			if( client.SendNetTableUpdates( m_NetStringTableManager ) == NST::SerializeResult::OVERFLOWED )
			{
				client.Disconnect( SVDisconnectCode::RELIABLE_CHANNEL_OVERFLOW );
			}
		}
	}
}

void CServer::DispatchClientMessages()
{
	for( size_t uiIndex = 0; uiIndex < m_uiMaxClients; ++uiIndex )
	{
		auto& client = m_pClients[ uiIndex ];

		if( client.IsConnected() )
		{
			auto& buffer = client.GetMessageBuffer();

			if( !buffer.HasOverflowed() )
			{
				ENetPacket* pPacket = enet_packet_create( buffer.GetData(), buffer.GetBytesInBuffer(), ENET_PACKET_FLAG_RELIABLE );

				buffer.ResetToStart();

				if( enet_peer_send( client.GetPeer(), NetChannel::DATA, pPacket ) == 0 )
				{
					client.SetLastMessageTime( WorldTime.GetCurrentTime() );
				}
				else
				{
					printf( "Error while sending packet to client!\n" );
				}
			}
			else
			{
				client.Disconnect( SVDisconnectCode::RELIABLE_CHANNEL_OVERFLOW );
			}
		}
	}
}