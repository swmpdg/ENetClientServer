#include <cassert>

#include <enet/enet.h>

#include <google/protobuf/message.h>

#include "networking/CNetworkBuffer.h"

#include "networking/NetworkUtils.h"

#include "networking/stringtable/PrivateNetStringTableConstants.h"

#include "messages/Noop.pb.h"

#include "messages/sv_cl_messages/ClientPrint.pb.h"
#include "messages/sv_cl_messages/ServerInfo.pb.h"
#include "messages/sv_cl_messages/NetTables.pb.h"
#include "messages/sv_cl_messages/NetTable.pb.h"
#include "messages/sv_cl_messages/FullyConnected.pb.h"
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

		const auto result = ProcessMessage( message, uiMessageSize, buffer );

		switch( result )
		{
		case ProcessResult::DESERIALIZE_FAILURE:
			{
				printf( "Encountered bad message %s (%u, %u bytes)\n", SVCLMessageToString( message ), message, uiMessageSize );
				m_Client.DisconnectFromServer( CLDisconnectCode::BAD_MESSAGE );
				return;
			}

		case ProcessResult::UNKNOWN_MESSAGE:
			{
				printf( "CCLServer::ProcessMessage: Invalid message %s (%u)\n", SVCLMessageToString( message ), message );
				return;
			}

		case ProcessResult::EXIT:
			{
				return;
			}
		}
	}
}

bool CCLServer::SendMessage( const CLSVMessage messageId, google::protobuf::Message& message )
{
	assert( IsConnected() );

	return SerializeToBuffer( messageId, message, m_MessageBuffer );
}

ProcessResult CCLServer::ProcessMessage( const SVCLMessage message, const size_t uiMessageSize, CNetworkBuffer& buffer )
{
	switch( message )
	{
	case SVCLMessage::NOOP:
		{
			messages::Noop noop;

			if( !DeserializeFromBuffer( buffer, uiMessageSize, noop ) )
				return ProcessResult::DESERIALIZE_FAILURE;

			break;
		}

	case SVCLMessage::CLIENTPRINT:
		{
			sv_cl_messages::ClientPrint print;

			if( !DeserializeFromBuffer( buffer, uiMessageSize, print ) )
				return ProcessResult::DESERIALIZE_FAILURE;

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

			if( !DeserializeFromBuffer( buffer, uiMessageSize, serverInfo ) )
				return ProcessResult::DESERIALIZE_FAILURE;

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

			if( !DeserializeFromBuffer( buffer, uiMessageSize, tables ) )
				return ProcessResult::DESERIALIZE_FAILURE;

			m_NetworkStringTableManager.ProcessNetTablesMessage( tables );

			cl_sv_messages::ConnectionCmd connCmd;

			connCmd.set_stage( static_cast<uint8_t>( ClientConnStage::NETTABLES ) );

			SendMessage( CLSVMessage::CONNECTIONCMD, connCmd );

			break;
		}

	case SVCLMessage::NETTABLE:
		{
			sv_cl_messages::NetTable table;

			if( !DeserializeFromBuffer( buffer, uiMessageSize, table ) )
				return ProcessResult::DESERIALIZE_FAILURE;

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
			sv_cl_messages::FullyConnected conn;

			if( !DeserializeFromBuffer( buffer, uiMessageSize, conn ) )
				return ProcessResult::DESERIALIZE_FAILURE;

			Connected();

			break;
		}

	case SVCLMessage::DISCONNECT:
		{
			sv_cl_messages::Disconnect disconnect;

			if( !DeserializeFromBuffer( buffer, uiMessageSize, disconnect ) )
				return ProcessResult::DESERIALIZE_FAILURE;

			strncpy( m_szDisconnectReason, disconnect.reason().c_str(), sizeof( m_szDisconnectReason ) );

			m_szDisconnectReason[ sizeof( m_szDisconnectReason ) - 1 ] = '\0';

			break;
		}

	default:
		{
			return ProcessResult::UNKNOWN_MESSAGE;
		}
	}

	return ProcessResult::SUCCESS;
}