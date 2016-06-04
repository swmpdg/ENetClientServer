#include <cassert>

#include <enet/enet.h>

#include <google/protobuf/message.h>

#include "networking/CNetworkBuffer.h"

#include "networking/NetworkUtils.h"

#include "networking/stringtable/PrivateNetStringTableConstants.h"

#include "messages/sv_cl_messages/ClientPrint.pb.h"
#include "messages/sv_cl_messages/ServerInfo.pb.h"
#include "messages/sv_cl_messages/NetTables.pb.h"
#include "messages/sv_cl_messages/NetTable.pb.h"
#include "messages/sv_cl_messages/Disconnect.pb.h"

#include "messages/cl_sv_messages/ConnectionCmd.pb.h"

#include "CClient.h"

#include "CCLServer.h"

CCLServer::CCLServer( CClient& client )
	: m_Client( client )
	, m_MessageBuffer( "ClientServerData", m_MessageBufData, sizeof( m_MessageBufData ) )
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
	assert( m_State == CLServerConnState::CONNECTED );

	m_State = CLServerConnState::FULLYCONNECTED;

	m_Client.Connected();
}

void CCLServer::PendingDisconnect()
{
	assert( m_State == CLServerConnState::CONNECTING || m_State == CLServerConnState::CONNECTED || m_State == CLServerConnState::FULLYCONNECTED );

	m_State = CLServerConnState::PENDINGDISCONNECT;
}

void CCLServer::Reset()
{
	assert( m_State != CLServerConnState::NOTCONNECTED );
	assert( m_pPeer );
	assert( m_pPeer->data == this );

	m_pPeer->data = nullptr;

	m_pPeer = nullptr;

	m_State = CLServerConnState::NOTCONNECTED;

	m_NetworkStringTableManager.Clear();

	memset( m_szIPAddress, 0, sizeof( m_szIPAddress ) );
	memset( m_szHostName, 0, sizeof( m_szHostName ) );
	memset( m_szDisconnectReason, 0, sizeof( m_szDisconnectReason ) );
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
	assert( IsConnected() );

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

	case SVCLMessage::SERVERINFO:
		{
			sv_cl_messages::ServerInfo serverInfo;

			serverInfo.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			strncpy( m_szIPAddress, serverInfo.ipaddress().c_str(), sizeof( m_szIPAddress ) );

			m_szIPAddress[ sizeof( m_szIPAddress ) - 1 ] = '\0';

			strncpy( m_szHostName, serverInfo.hostname().c_str(), sizeof( m_szHostName ) );

			m_szHostName[ sizeof( m_szHostName ) - 1 ] = '\0';

			cl_sv_messages::ConnectionCmd connCmd;

			connCmd.set_stage( static_cast<uint8_t>( ClientConnStage::SERVER_INFO ) );

			SendMessage( CLSVMessage::CONNECTIONCMD, connCmd );

			break;
		}

	case SVCLMessage::NETTABLES:
		{
			sv_cl_messages::NetTables tables;

			tables.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			m_NetworkStringTableManager.ProcessNetTablesMessage( tables );

			cl_sv_messages::ConnectionCmd connCmd;

			connCmd.set_stage( static_cast<uint8_t>( ClientConnStage::NETTABLES ) );

			SendMessage( CLSVMessage::CONNECTIONCMD, connCmd );

			break;
		}

	case SVCLMessage::NETTABLE:
		{
			sv_cl_messages::NetTable table;

			table.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			m_NetworkStringTableManager.ProcessNetTableMessage( table );

			if( m_State == CLServerConnState::CONNECTED )
			{
				cl_sv_messages::ConnectionCmd connCmd;

				connCmd.set_stage( static_cast<uint8_t>( ClientConnStage::NETTABLES ) );

				switch( table.command() )
				{
				case sv_cl_messages::NetTable_Command_UPDATE:
					{
						auto pStateField = connCmd.mutable_nettablestate();

						pStateField->set_tableid( table.tableid() );
						pStateField->set_stringindex( m_NetworkStringTableManager.GetTableByIndex( NST::TableIDToIndex( table.tableid() ) )->GetStringCount() );

						break;
					}
				}

				SendMessage( CLSVMessage::CONNECTIONCMD, connCmd );
			}

			break;
		}

	case SVCLMessage::FULLYCONNECTED:
		{
			Connected();

			break;
		}

	case SVCLMessage::DISCONNECT:
		{
			sv_cl_messages::Disconnect disconnect;

			disconnect.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			strncpy( m_szDisconnectReason, disconnect.reason().c_str(), sizeof( m_szDisconnectReason ) );

			m_szDisconnectReason[ sizeof( m_szDisconnectReason ) - 1 ] = '\0';

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