#ifndef CSVCLIENT_H
#define CSVCLIENT_H

#include <cstdint>

#include <enet/enet.h>

#include <google/protobuf/message.h>

#include "networking/NetworkConstants.h"

#include "networking/CNetworkBuffer.h"

#include "networking/stringtable/PrivateNetStringTableConstants.h"

#undef SendMessage

class CServer;

class CServerNetworkStringTableManager;

namespace cl_sv_messages
{
class ConnectionCmd;
}

/**
*	The server's client's connection state.
*/
enum class SVClientConnState
{
	FREE = 0,
	CONNECTING,
	CONNECTED,
	PENDINGDISCONNECT
};

/**
*	The server's representation of a client.
*/
class CSVClient final
{
public:
	/**
	*	Constructor.
	*/
	CSVClient();

	/**
	*	Destructor.
	*/
	~CSVClient();

	/**
	*	@return The client's connection state.
	*/
	SVClientConnState GetConnectionState() const { return m_State; }

	/**
	*	@return If the client is currently connecting to the server, the client's connection stage.
	*/
	ClientConnStage GetConnectionStage() const { return m_ConnectionStage; }

	/**
	*	@return Whether this client is connected to this server in any way.
	*/
	bool IsConnected() const { return m_State == SVClientConnState::CONNECTING || m_State == SVClientConnState::CONNECTED; }

	/**
	*	@return Whether this client is fully connected. A fully connected client is playing the game.
	*/
	bool IsFullyConnected() const { return m_State == SVClientConnState::CONNECTED; }

	/**
	*	@return The network peer.
	*/
	ENetPeer* GetPeer() const { return m_pPeer; }

	/**
	*	@return The reliable message buffer.
	*/
	const CNetworkBuffer& GetMessageBuffer() const { return m_MessageBuffer; }

	/**
	*	@copydoc GetMessageBuffer() const
	*/
	CNetworkBuffer& GetMessageBuffer() { return m_MessageBuffer; }

	/**
	*	@return The time at which the client connected.
	*/
	float GetConnectTime() const { return m_flConnectTime; }

	/**
	*	@return The last time we sent a message to this client.
	*/
	float GetLastMessageTime() const { return m_flLastMessageTime; }

	/**
	*	Sets the last time we sent a message to the client.
	*	@see GetLastMessageTime() const
	*/
	void SetLastMessageTime( const float flLastMessageTime )
	{
		m_flLastMessageTime = flLastMessageTime;
	}

	float GetLastNetTableTime() const { return m_flLastNetTableTime; }

	/**
	*	Called when a client has connected and this client object has been assigned to it.
	*	@param pPeer Peer to associate with this client.
	*/
	void Initialize( ENetPeer* pPeer );

	/**
	*	Sends server info to this client. Only valid during connecting state.
	*/
	void SendServerInfo( CServer& server );

	/**
	*	Called when the client finishes connecting.
	*/
	void Connected();

	/**
	*	Disconnects the client.
	*/
	void Disconnect( const SVDisconnectCode::SVDisconnectCode disconnectCode, const char* const pszReason = nullptr );

	/**
	*	Called when this client has disconnected.
	*/
	void Reset();

	/**
	*	Sends a message to this client.
	*	@param messageId ID of the message to send.
	*	@param message Message to send.
	*	@return true on success, false otherwise.
	*/
	bool SendMessage( const SVCLMessage messageId, google::protobuf::Message& message );

	/**
	*	Sends a message to this client.
	*	@param buffer Buffer containing the message.
	*	@return true on success, false otherwise.
	*/
	bool SendMessage( const CNetworkBuffer& buffer );

	/**
	*	Send network string table updates to the client.
	*/
	NST::SerializeResult SendNetTableUpdates( CServerNetworkStringTableManager& manager );

	/**
	*	Process messages sent by this client.
	*/
	void ProcessMessages( CServer& server, CNetworkBuffer& buffer );

private:
	/**
	*	Process a single message sent by this client.
	*	@param message Message ID.
	*	@param uiMessageSize Message size, in bytes.
	*	@param buffer Buffer containing the message.
	*/
	bool ProcessMessage( CServer& server, const CLSVMessage message, const size_t uiMessageSize, CNetworkBuffer& buffer );

	/**
	*	Sends all network string tables to the client. Data is spread out over multiple messages if necessary, requiring multiple calls to this method.
	*	@return true if data was sent, false otherwise.
	*/
	NST::SerializeResult SendNetTables( CServerNetworkStringTableManager& manager, const cl_sv_messages::ConnectionCmd& connCmd );

private:
	SVClientConnState m_State = SVClientConnState::FREE;

	/**
	*	If the client is connecting to the server (SVClientConnState::CONNECTING), this is the connection stage it is current at.
	*/
	ClientConnStage m_ConnectionStage = ClientConnStage::NONE;

	ENetPeer* m_pPeer = nullptr;

	uint8_t m_MessageBufData[ MAX_DATAGRAM ];

	CNetworkBuffer m_MessageBuffer;

	float m_flConnectTime = 0;

	float m_flLastMessageTime = 0;

	float m_flLastNetTableTime = 0;

private:
	CSVClient( const CSVClient& ) = delete;
	CSVClient& operator=( const CSVClient& ) = delete;
};

#endif //CSVCLIENT_H