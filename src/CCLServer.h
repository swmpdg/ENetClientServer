#ifndef CCLSERVER_H
#define CCLSERVER_H

#include <cstdint>

#include <enet/enet.h>

#include "NetworkConstants.h"

#include "utility/CNetworkBuffer.h"

#include "utility/CNetworkStringTableManager.h"
#include "CClientNetworkStringTableManager.h"

#undef SendMessage

/**
*	The client's server's connection state.
*/
enum class CLServerConnState
{
	NOTCONNECTED = 0,
	CONNECTING,
	CONNECTED,
	PENDINGDISCONNECT
};

/**
*	The client's representation of a server it's connected to.
*/
class CCLServer final
{
public:
	/**
	*	Constructor.
	*/
	CCLServer();

	/**
	*	Destructor.
	*/
	~CCLServer();

	/**
	*	@return The server's connection state.
	*/
	CLServerConnState GetConnectionState() const { return m_State; }

	/**
	*	@return Whether the local client is connected to this server in any way.
	*/
	bool IsConnected() const { return m_State == CLServerConnState::CONNECTING || m_State == CLServerConnState::CONNECTED || m_State == CLServerConnState::PENDINGDISCONNECT; }

	/**
	*	@return Whether the local client is fully connected to this server.
	*/
	bool IsFullyConnected() const { return m_State == CLServerConnState::CONNECTED; }

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
	*	@return The client's network string table manager.
	*/
	CNetworkStringTableManager& GetNetStringTableManager() { return m_NetworkStringTableManager; }

	/**
	*	Initializes this server handler.
	*	@param pPeer Peer to associate with this server.
	*/
	void Initialize( ENetPeer* pPeer );

	/**
	*	Called when the client has fully connected to this server.
	*/
	void Connected();

	/**
	*	Called when the client has issued a disconnect and is awaiting disconnect.
	*/
	void PendingDisconnect();

	/**
	*	Resets this server handler to defaults.
	*/
	void Reset();

	/**
	*	Processes messages sent by this server to the local client.
	*/
	void ProcessMessages( CNetworkBuffer& buffer );

	/**
	*	Sends a message to the server.
	*	@param messageId Message ID.
	*	@param message Message to send.
	*	@return true on success, false otherwise.
	*/
	bool SendMessage( const CLSVMessage messageId, google::protobuf::Message& message );

private:

	/**
	*	Processes a single message.
	*	@param message Message ID.
	*	@param uiMessageSize Size of the message, in bytes.
	*	@param buffer Buffer containing the message.
	*/
	void ProcessMessage( const SVCLMessage message, const size_t uiMessageSize, CNetworkBuffer& buffer );

private:
	CLServerConnState m_State = CLServerConnState::NOTCONNECTED;

	ENetPeer* m_pPeer = nullptr;

	uint8_t m_MessageBufData[ MAX_DATAGRAM ];

	CNetworkBuffer m_MessageBuffer;

	CClientNetworkStringTableManager m_NetworkStringTableManager;

private:
	CCLServer( const CCLServer& ) = delete;
	CCLServer& operator=( const CCLServer& ) = delete;
};

#endif //CCLSERVER_H