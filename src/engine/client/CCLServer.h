#ifndef ENGINE_CLIENT_CCLSERVER_H
#define ENGINE_CLIENT_CCLSERVER_H

#include <cstdint>

#include <enet/forward.h>

#include "shared/Utility.h"

#include "networking/NetworkConstants.h"

#include "networking/CNetworkBuffer.h"

#include "networking/stringtable/CNetworkStringTableManager.h"
#include "CClientNetworkStringTableManager.h"

/**
*	@ingroup EngineClient
*
*	@{
*/

/**
*	The client's server's connection state.
*/
enum class CLServerConnState
{
	/**
	*	Not connected to the server.
	*/
	NOTCONNECTED = 0,

	/**
	*	Currently connecting to the server, awaiting ENet response.
	*/
	CONNECTING,

	/**
	*	Connected, communicating serfer and client info.
	*/
	CONNECTED,

	/**
	*	Fully connected to the server.
	*/
	FULLYCONNECTED,

	/**
	*	Submitted disconnect request and awaiting response.
	*/
	PENDINGDISCONNECT
};

class CClient;

/**
*	The client's representation of a server it's connected to.
*/
class CCLServer final
{
public:
	/**
	*	Constructor.
	*/
	CCLServer( CClient& client );

	/**
	*	Destructor.
	*/
	~CCLServer();

	/**
	*	@return The server's connection state.
	*/
	CLServerConnState GetConnectionState() const { return m_State; }

	/**
	*	Sets the server connection state.
	*/
	void SetConnectionState( const CLServerConnState state ) { m_State = state; }

	/**
	*	@return Whether the local client is connected to this server in any way.
	*/
	bool IsConnected() const { return 
		m_State == CLServerConnState::CONNECTED || 
		m_State == CLServerConnState::FULLYCONNECTED; }

	/**
	*	@return Whether the local client is fully connected to this server.
	*/
	bool IsFullyConnected() const { return m_State == CLServerConnState::FULLYCONNECTED; }

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
	CClientNetworkStringTableManager& GetNetStringTableManager() { return m_NetworkStringTableManager; }

	/**
	*	@return This server's IP address.
	*/
	const char* GetIPAddress() const { return m_szIPAddress; }

	/**
	*	@return This server's host name.
	*/
	const char* GetHostName() const { return m_szHostName; }

	/**
	*	@return The last disconnect reason, if any.
	*/
	const char* GetDisconnectReason() const { return m_szDisconnectReason; }

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
	*	@return Process result.
	*/
	ProcessResult ProcessMessage( const SVCLMessage message, const size_t uiMessageSize, CNetworkBuffer& buffer );

private:
	CClient& m_Client;

	CLServerConnState m_State = CLServerConnState::NOTCONNECTED;

	ENetPeer* m_pPeer = nullptr;

	uint8_t m_MessageBufData[ MAX_DATAGRAM ];

	CNetworkBuffer m_MessageBuffer;

	CClientNetworkStringTableManager m_NetworkStringTableManager;

	char m_szIPAddress[ MAX_BUFFER_LENGTH ] = { '\0' };

	char m_szHostName[ MAX_BUFFER_LENGTH ] = { '\0' };

	char m_szDisconnectReason[ MAX_BUFFER_LENGTH ] = { '\0' };

private:
	CCLServer( const CCLServer& ) = delete;
	CCLServer& operator=( const CCLServer& ) = delete;
};

/** @} */

#endif //ENGINE_CLIENT_CCLSERVER_H