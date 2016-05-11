#include <cassert>

#include "utility/CNetworkBuffer.h"

#include "CServer.h"

#include "messages/cl_sv_messages/ClientCmd.pb.h"
#include "messages/sv_cl_messages/ClientPrint.pb.h"

#include "CSVClient.h"

CSVClient::~CSVClient()
{
	assert( m_State == SVClientConnState::FREE );
	assert( !m_pPeer );
}

void CSVClient::Initialize( ENetPeer* pPeer )
{
	assert( m_State == SVClientConnState::FREE );

	assert( pPeer );

	m_pPeer = pPeer;

	m_State = SVClientConnState::CONNECTING;

	m_pPeer->data = this;
}

void CSVClient::Connected()
{
	assert( m_State == SVClientConnState::CONNECTING );

	m_State = SVClientConnState::CONNECTED;
}

void CSVClient::Reset()
{
	assert( IsConnected() );
	assert( m_pPeer );
	assert( m_pPeer->data == this );

	m_pPeer->data = nullptr;

	m_pPeer = nullptr;

	m_State = SVClientConnState::FREE;
}

void CSVClient::ProcessMessages( CServer& server, CNetworkBuffer& buffer )
{
	CLSVMessage message;

	while( ( message = static_cast<CLSVMessage>( buffer.ReadByte() ) ) != CLSVMessage::NONE )
	{
		if( buffer.HasOverflowed() )
		{
			break;
		}

		const size_t uiMessageSize = buffer.ReadUnsignedBitLong( 32 );

		ProcessMessage( server, message, uiMessageSize, buffer );
	}
}

void CSVClient::ProcessMessage( CServer& server, const CLSVMessage message, const size_t uiMessageSize, CNetworkBuffer& buffer )
{
	switch( message )
	{
	case CLSVMessage::NOOP:
		{
			break;
		}

	case CLSVMessage::CLIENTCMD:
		{
			cl_sv_messages::ClientCmd cmd;

			cmd.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			//TODO: handle client command
			printf( "from client: %s", cmd.command().c_str() );

			printf( "server: Message Size: %u\n", uiMessageSize );

			sv_cl_messages::ClientPrint print;

			print.set_type( sv_cl_messages::ClientPrint_Type_CONSOLE );

			print.set_message( "Message received\n" );

			server.SendMessage( SVCLMessage::CLIENTPRINT, print );

			break;
		}
	}

	buffer.ReadAndDiscardBytes( uiMessageSize );
}