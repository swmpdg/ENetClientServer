#include <cassert>

#include "networking/CNetworkBuffer.h"

#include "networking/NetworkUtils.h"

#include "messages/sv_cl_messages/ClientPrint.pb.h"
#include "messages/sv_cl_messages/NetTable.pb.h"

#include "CCLServer.h"

CCLServer::CCLServer()
	: m_MessageBuffer( "ClientServerData", m_MessageBufData, sizeof( m_MessageBufData ) )
{
}

CCLServer::~CCLServer()
{
	assert( !m_pPeer );
}

void CCLServer::Initialize( ENetPeer* pPeer )
{
	assert( m_State == CLServerConnState::NOTCONNECTED );

	assert( pPeer );

	m_pPeer = pPeer;

	m_State = CLServerConnState::CONNECTING;

	m_pPeer->data = this;

	m_MessageBuffer.ResetToStart();
}

void CCLServer::Connected()
{
	assert( m_State == CLServerConnState::CONNECTING );

	m_State = CLServerConnState::CONNECTED;
}

void CCLServer::PendingDisconnect()
{
	assert( m_State == CLServerConnState::CONNECTING || m_State == CLServerConnState::CONNECTED );

	m_State = CLServerConnState::PENDINGDISCONNECT;
}

void CCLServer::Reset()
{
	assert( IsConnected() );
	assert( m_pPeer );
	assert( m_pPeer->data == this );

	m_pPeer->data = nullptr;

	m_pPeer = nullptr;

	m_State = CLServerConnState::NOTCONNECTED;

	m_NetworkStringTableManager.Clear();
}

void CCLServer::ProcessMessages( CNetworkBuffer& buffer )
{
	SVCLMessage message;

	while( buffer.GetBytesLeft() > 0 && ( message = static_cast<SVCLMessage>( buffer.ReadByte() ) ) != SVCLMessage::NONE )
	{
		if( buffer.HasOverflowed() )
		{
			printf( "CCLServer::ProcessMessages: Overflowed while reading from buffer!\n" );
			break;
		}

		const size_t uiMessageSize = buffer.ReadUnsignedBitLong( NETMSG_SIZE_BITS );

		ProcessMessage( message, uiMessageSize, buffer );
	}
}

bool CCLServer::SendMessage( const CLSVMessage messageId, google::protobuf::Message& message )
{
	assert( IsFullyConnected() );

	return SerializeToBuffer( messageId, message, m_MessageBuffer );
}

void CCLServer::ProcessMessage( const SVCLMessage message, const size_t uiMessageSize, CNetworkBuffer& buffer )
{
	switch( message )
	{
	case SVCLMessage::NOOP:
		{
			break;
		}

	case SVCLMessage::CLIENTPRINT:
		{
			sv_cl_messages::ClientPrint print;

			print.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			switch( print.type() )
			{
			case sv_cl_messages::ClientPrint_Type_CONSOLE:
				{
					//TODO: print to console
					printf( "from server: %s", print.message().c_str() );

					break;
				}
			}

			break;
		}

	case SVCLMessage::NETTABLE:
		{
			sv_cl_messages::NetTable table;

			table.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			m_NetworkStringTableManager.ProcessNetTableMessage( table );

			break;
		}

	default:
		{
			printf( "CCLServer::ProcessMessage: Invalid message %d\n", message );
			break;
		}
	}

	buffer.ReadAndDiscardBytes( uiMessageSize );
}