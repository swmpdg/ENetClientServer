#include <cassert>
#include <memory>

#include <enet/enet.h>

//Undefines Windows header stuff so it doesn't interfere.
#include "shared/Platform.h"

#include "networking/NetworkUtils.h"

#include "game/shared/client/IGameClientInterface.h"

#include "messages/Noop.pb.h"

#include "CClient.h"

CClient::CClient()
	: m_Server( *this )
{
}

CClient::~CClient()
{
	assert( !m_pHost );
}

bool CClient::Connect( const CreateInterfaceFn* factories, const size_t uiNumFactories )
{
	assert( factories );
	assert( uiNumFactories );

	for( size_t uiIndex = 0; uiIndex < uiNumFactories; ++uiIndex )
	{
		auto factory = factories[ uiIndex ];

		if( !m_pGameClient )
		{
			m_pGameClient = static_cast<IGameClientInterface*>( factory( IGAMECLIENTINTERFACE_NAME, nullptr ) );

			m_Server.GetNetStringTableManager().SetGameClient( m_pGameClient );
		}
	}

	return m_pGameClient != nullptr;
}

bool CClient::Initialize()
{
	assert( !m_bInitialized );

	m_bInitialized = true;

	m_pHost = enet_host_create( nullptr, 1, NetChannel::COUNT, 0, 0 );

	if( !m_pHost )
		return false;

	return true;
}

void CClient::Shutdown()
{
	assert( m_bInitialized );

	m_bInitialized = false;

	if( m_Server.IsConnected() )
	{
		enet_peer_reset( m_Server.GetPeer() );
		m_Server.Reset();
	}

	if( m_pHost )
	{
		enet_host_destroy( m_pHost );
		m_pHost = nullptr;
	}
}

bool CClient::ConnectToServer( const char* pszAddress, const enet_uint16 port )
{
	assert( pszAddress );
	assert( !IsConnected() );

	ENetAddress address;

	enet_address_set_host( &address, pszAddress );

	address.port = port;

	auto pPeer = enet_host_connect( m_pHost, &address, NetChannel::COUNT, 0 );

	if( !pPeer )
		return false;

	m_Server.Initialize( pPeer );

	return true;
}

void CClient::Connected()
{
	m_pGameClient->ClientPutInServer();

	printf( "Connected to server %s(%s)\n", m_Server.GetHostName(), m_Server.GetIPAddress() );
}

void CClient::DisconnectFromServer( const CLDisconnectCode::CLDisconnectCode disconnectCode )
{
	if( !IsConnected() )
	{
		return;
	}

	//TODO: there is no way to tell the difference between client and server disconnects. Should add a way.

	enet_peer_disconnect( m_Server.GetPeer(), disconnectCode );

	m_Server.PendingDisconnect();
}

bool CClient::SendMessage( const CLSVMessage messageId, google::protobuf::Message& message )
{
	assert( IsConnected() );

	return m_Server.SendMessage( messageId, message );
}

void CClient::RunFrame()
{
	ProcessNetworkEvents();

	DispatchServerMessages();
}

bool CClient::ClientCommand( const CCommand& command )
{
	if( m_pGameClient->ClientCommand( command ) )
		return true;

	return false;
}

void CClient::ProcessNetworkEvents()
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
				//Connected to server.

				assert( m_Server.GetConnectionState() == CLServerConnState::CONNECTING );

				if( m_pGameClient->ClientConnected() )
				{
					m_Server.SetConnectionState( CLServerConnState::CONNECTED );
				}
				else
				{
					DisconnectFromServer();
				}

				break;
			}

		case ENET_EVENT_TYPE_DISCONNECT:
			{
				if( event.peer->data )
				{
					CCLServer* const pServer = reinterpret_cast<CCLServer*>( event.peer->data );

					assert( pServer->GetConnectionState() != CLServerConnState::NOTCONNECTED );

					m_pGameClient->ClientDisconnected( pServer->IsFullyConnected() );

					const char* pszReason;

					auto pszDisconnectReason = m_Server.GetDisconnectReason();

					if( pszDisconnectReason && *pszDisconnectReason )
					{
						pszReason = pszDisconnectReason;
					}
					else
					{
						pszReason = SVDisconnectCode::ToString( static_cast<SVDisconnectCode::SVDisconnectCode>( event.data ) );
					}

					printf( "You have been disconnected from the server.\nThe reason given was:\n%s\n", pszReason );

					pServer->Reset();
				}

				break;
			}

		case ENET_EVENT_TYPE_RECEIVE:
			{
				ProcessPacket( *reinterpret_cast<CCLServer*>( event.peer->data ), event.packet );

				enet_packet_destroy( event.packet );
				break;
			}
		}
	}
}

void CClient::ProcessPacket( CCLServer& server, ENetPacket* pPacket )
{
	assert( pPacket );

	CNetworkBuffer buffer{ "CClient::ProcessPacket_Buffer", pPacket->data, pPacket->dataLength };

	server.ProcessMessages( buffer );
}

void CClient::DispatchServerMessages()
{
	if( m_Server.IsConnected() )
	{
		auto& buffer = m_Server.GetMessageBuffer();

		//Ensure the buffer is never empty.
		if( buffer.GetBitsInBuffer() == 0 )
		{
			messages::Noop noop;

			m_Server.SendMessage( CLSVMessage::NOOP, noop );
		}

		ENetPacket* pPacket = enet_packet_create( buffer.GetData(), buffer.GetBytesInBuffer(), ENET_PACKET_FLAG_RELIABLE );

		if( enet_peer_send( m_Server.GetPeer(), NetChannel::DATA, pPacket ) != 0 )
		{
			printf( "Error while sending packet to server!\n" );
		}

		buffer.ResetToStart();
	}
}