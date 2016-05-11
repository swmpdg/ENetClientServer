#include <cassert>

#include <memory>

#include "utility/CNetworkBuffer.h"

#include "CClient.h"

CClient::~CClient()
{
	assert( !m_pHost );
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

void CClient::DisconnectFromServer()
{
	if( !IsConnected() )
	{
		return;
	}

	enet_peer_disconnect( m_Server.GetPeer(), CLDisconnectCode::USER_DISCONNECTED );
}

bool CClient::SendMessage( const CLSVMessage messageId, google::protobuf::Message& message )
{
	assert( IsConnected() );

	uint8_t szBuffer[ 80000 ];

	CNetworkBuffer buffer( "CL_Send_Buffer", szBuffer, sizeof( szBuffer ) );

	buffer.WriteByte( static_cast<int>( messageId ) );

	const size_t messageSize = message.ByteSize();

	buffer.WriteUnsignedBitLong( messageSize, 32 );

	//TODO: avoid memory allocation.
	auto messageBuf = std::make_unique<uint8_t[]>( messageSize );

	if( !message.SerializeToArray( messageBuf.get(), messageSize ) )
	{
		printf( "Failed to serialize message\n" );
		return false;
	}

	buffer.WriteBytes( messageBuf.get(), messageSize );

	if( buffer.HasOverflowed() )
	{
		printf( "Message buffer overflowed\n" );
		return false;
	}

	//TODO: this should be appending reliable messages to the server buffer, and then send it all at once.
	ENetPacket* pPacket = enet_packet_create( buffer.GetData(), buffer.GetBytesInBuffer(), ENET_PACKET_FLAG_RELIABLE );

	return enet_peer_send( m_Server.GetPeer(), NetChannel::DATA, pPacket ) == 0;
}

void CClient::RunFrame()
{
	ProcessNetworkEvents();
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

				//TODO: this should be called when the client has finished connecting (got server info, downloaded resources, etc).
				m_Server.Connected();

				break;
			}

		case ENET_EVENT_TYPE_DISCONNECT:
			{
				if( event.peer->data )
				{
					CCLServer* const pServer = reinterpret_cast<CCLServer*>( event.peer->data );

					assert( pServer->IsConnected() );

					//TODO: call ClientDisconnected here.

					pServer->Reset();
				}

				break;
			}

		case ENET_EVENT_TYPE_RECEIVE:
			{
				ProcessPacket( *reinterpret_cast<CCLServer*>( event.peer->data ), event.packet );
				break;
			}
		}
	}
}

void CClient::ProcessPacket( CCLServer& server, ENetPacket* pPacket )
{
	assert( pPacket );

	CNetworkBuffer buffer{ "CL_Packet", pPacket->data, pPacket->dataLength };

	server.ProcessMessages( buffer );
}