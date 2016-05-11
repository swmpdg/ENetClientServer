#include <cassert>

#include "utility/CNetworkBuffer.h"

#include "messages/sv_cl_messages/ClientPrint.pb.h"

#include "CCLServer.h"

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
}

void CCLServer::Connected()
{
	assert( m_State == CLServerConnState::CONNECTING );

	m_State = CLServerConnState::CONNECTED;
}

void CCLServer::Reset()
{
	assert( IsConnected() );
	assert( m_pPeer );
	assert( m_pPeer->data == this );

	m_pPeer->data = nullptr;

	m_pPeer = nullptr;

	m_State = CLServerConnState::NOTCONNECTED;
}

void CCLServer::ProcessMessages( CNetworkBuffer& buffer )
{
	SVCLMessage message;

	while( buffer.GetBytesLeft() > 0 && ( message = static_cast<SVCLMessage>( buffer.ReadByte() ) ) != SVCLMessage::NONE )
	{
		if( buffer.HasOverflowed() )
		{
			break;
		}

		const size_t uiMessageSize = buffer.ReadUnsignedBitLong( 32 );

		ProcessMessage( message, uiMessageSize, buffer );
	}
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
	}

	buffer.ReadAndDiscardBytes( uiMessageSize );
}