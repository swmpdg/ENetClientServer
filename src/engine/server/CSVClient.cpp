#include <cassert>
#include <memory>

#include <enet/enet.h>

#include "networking/CNetworkBuffer.h"

#include "networking/NetworkUtils.h"

#include "networking/stringtable/PrivateNetStringTableConstants.h"

#include "utility/CWorldTime.h"

#include "shared/Utility.h"

#include "game/shared/server/IGameServerInterface.h"

#include "CServer.h"

#include "CServerNetworkStringTableManager.h"

#include "messages/sv_cl_messages/ClientPrint.pb.h"
#include "messages/sv_cl_messages/ServerInfo.pb.h"
#include "messages/sv_cl_messages/FullyConnected.pb.h"

#include "messages/cl_sv_messages/ClientCmd.pb.h"
#include "messages/cl_sv_messages/ConnectionCmd.pb.h"

#include "CSVClient.h"

#undef GetCurrentTime

CSVClient::CSVClient()
	: m_MessageBuffer( "ServerClientMessage", m_MessageBufData, sizeof( m_MessageBufData ) )
{
}

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

	m_MessageBuffer.ResetToStart();

	m_flConnectTime = WorldTime.GetCurrentTime();

	m_flLastMessageTime = 0;

	m_flLastNetTableTime = 0;
}

void CSVClient::SendServerInfo( CServer& server )
{
	assert( m_State == SVClientConnState::CONNECTING );

	sv_cl_messages::ServerInfo serverInfo;

	ENetAddress address;

	//TODO: drop client on failure.
	if( enet_address_set_host( &address, "localhost" ) < 0 )
		return;

	char szIPAddress[ MAX_BUFFER_LENGTH ];

	if( enet_address_get_host_ip( &address, szIPAddress, sizeof( szIPAddress ) ) < 0 )
		return;

	serverInfo.set_ipaddress( szIPAddress );
	//TODO: get host name from somewhere
	serverInfo.set_hostname( "my host name" );

	SendMessage( SVCLMessage::SERVERINFO, serverInfo );

	m_ConnectionStage = ClientConnStage::SERVER_INFO;
}

void CSVClient::Connected()
{
	assert( m_State == SVClientConnState::CONNECTING );

	m_State = SVClientConnState::CONNECTED;

	sv_cl_messages::FullyConnected conn;

	SendMessage( SVCLMessage::FULLYCONNECTED, conn );

	//Sends any changes made during connection.
	//TODO: if a large amount of changes were made, this could cause overflows.
	m_flLastNetTableTime = m_flConnectTime;
}

void CSVClient::Reset()
{
	assert( IsConnected() );
	assert( m_pPeer );
	assert( m_pPeer->data == this );

	m_pPeer->data = nullptr;

	m_pPeer = nullptr;

	m_State = SVClientConnState::FREE;

	m_flLastMessageTime = 0;

	m_flLastNetTableTime = 0;
}

bool CSVClient::SendMessage( const SVCLMessage messageId, google::protobuf::Message& message )
{
	return SerializeToBuffer( messageId, message, m_MessageBuffer );
}

bool CSVClient::SendMessage( const CNetworkBuffer& buffer )
{
	return m_MessageBuffer.WriteBits( buffer.GetData(), buffer.GetBitsInBuffer() );
}

void CSVClient::SendNetTableUpdates( CServerNetworkStringTableManager& manager )
{
	manager.Serialize( GetMessageBuffer(), m_flLastNetTableTime );

	m_flLastNetTableTime = WorldTime.GetCurrentTime();
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

		const size_t uiMessageSize = buffer.ReadUnsignedBitLong( NETMSG_SIZE_BITS );

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

			server.SendBroadcastMessage( SVCLMessage::CLIENTPRINT, print );

			break;
		}

	case CLSVMessage::CONNECTIONCMD:
		{
			cl_sv_messages::ConnectionCmd connCmd;

			connCmd.ParseFromArray( buffer.GetCurrentData(), uiMessageSize );

			const auto connStage = static_cast<ClientConnStage>( connCmd.stage() );

			assert( m_ConnectionStage == connStage );

			switch( connStage )
			{
			case ClientConnStage::SERVER_INFO:
				{
					//Server info received, move on to net tables.
					m_ConnectionStage = ClientConnStage::NETTABLES;

					server.GetNetStringTableManager().WriteNetTableCreateMessages( GetMessageBuffer() );

					break;
				}

			case ClientConnStage::NETTABLES:
				{
					if( !SendNetTables( server.GetNetStringTableManager(), connCmd ) )
					{
						m_ConnectionStage = ClientConnStage::NONE;

						Connected();

						server.GetGameServer()->ClientPutInServer();
					}

					break;
				}
			}

			break;
		}
	}

	buffer.ReadAndDiscardBytes( uiMessageSize );
}

bool CSVClient::SendNetTables( CServerNetworkStringTableManager& manager, const cl_sv_messages::ConnectionCmd& connCmd )
{
	size_t uiTable = 0;

	size_t uiFirstString = 0;

	if( connCmd.has_nettablestate() )
	{
		const auto& state = connCmd.nettablestate();

		uiTable = NST::TableIDToIndex( state.tableid() );
		uiFirstString = state.stringindex();
	}

	bool bSent = false;

	while( manager.GetTableByIndex( uiTable ) )
	{
		bSent = manager.WriteBaseline( uiTable, uiFirstString, 0, GetMessageBuffer() );

		if( bSent )
			break;

		++uiTable;
		uiFirstString = 0;
	}

	return bSent;
}