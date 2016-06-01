#include <cassert>
#include <cstdio>
#include <memory>

#include "NetworkConstants.h"

#include "utility/CNetworkBuffer.h"

#include "utility/NetworkUtils.h"

#include "CSVClient.h"

#include "CServer.h"

CServer::~CServer()
{
	assert( !m_bInitialized );
	assert( !m_pHost );
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

void CServer::Shutdown()
{
	assert( m_bInitialized );

	m_bInitialized = false;

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

				for( size_t uiIndex = 0; uiIndex < m_uiMaxClients; ++uiIndex )
				{
					auto& client = m_pClients[ uiIndex ];

					if( !client.IsConnected() )
					{
						//TODO: assign edict
						client.Initialize( event.peer );

						//TODO: Call ClientConnect here.

						//TODO: This should be called after the client has finished connecting (got server info, downloaded files, etc)
						client.Connected();

						bAssignedSlot = true;

						printf( "Client connected\n" );

						break;
					}
				}

				if( !bAssignedSlot )
				{
					enet_peer_disconnect( event.peer, SVDisconnectCode::NO_FREE_SLOTS );
				}

				break;
			}

		case ENET_EVENT_TYPE_DISCONNECT:
			{
				if( event.peer->data )
				{
					CSVClient* const pClient = reinterpret_cast<CSVClient*>( event.peer->data );

					assert( ( pClient - m_pClients ) >= 0 && static_cast<size_t>( pClient - m_pClients ) < m_uiMaxClients );
					assert( pClient->IsConnected() );

					printf( "Client disconnected\n" );

					//TODO: call ClientDisconnect here if it was fully connected before

					pClient->Reset();
				}

				break;
			}

		case ENET_EVENT_TYPE_RECEIVE:
			{
				ProcessPacket( *reinterpret_cast<CSVClient*>( event.peer->data ), event.packet );
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

void CServer::DispatchClientMessages()
{
	for( size_t uiIndex = 0; uiIndex < m_uiMaxClients; ++uiIndex )
	{
		auto& client = m_pClients[ uiIndex ];

		if( client.IsFullyConnected() )
		{
			auto& buffer = client.GetMessageBuffer();

			ENetPacket* pPacket = enet_packet_create( buffer.GetData(), buffer.GetBytesInBuffer(), ENET_PACKET_FLAG_RELIABLE );

			buffer.ResetToStart();

			if( enet_peer_send( client.GetPeer(), NetChannel::DATA, pPacket ) != 0 )
			{
				printf( "Error while sending packet to client!\n" );
			}
		}
	}
}