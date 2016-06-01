#ifndef CSVCLIENT_H
#define CSVCLIENT_H

#include <cstdint>

#include <enet/enet.h>

#include <google/protobuf/message.h>

#include "NetworkConstants.h"

#include "utility/CNetworkBuffer.h"

#undef SendMessage

class CServer;

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
	*	@return Whether this client is connected to this server in any way.
	*/
	bool IsConnected() const { return m_State == SVClientConnState::CONNECTING || m_State == SVClientConnState::CONNECTED || m_State == SVClientConnState::PENDINGDISCONNECT; }

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
	*	Called when a client has connected and this client object has been assigned to it.
	*	@param pPeer Peer to associate with this client.
	*/
	void Initialize( ENetPeer* pPeer );

	/**
	*	Called when the client finishes connecting.
	*/
	void Connected();

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
	void ProcessMessage( CServer& server, const CLSVMessage message, const size_t uiMessageSize, CNetworkBuffer& buffer );

private:
	SVClientConnState m_State = SVClientConnState::FREE;

	ENetPeer* m_pPeer = nullptr;

	uint8_t m_MessageBufData[ MAX_DATAGRAM ];

	CNetworkBuffer m_MessageBuffer;

private:
	CSVClient( const CSVClient& ) = delete;
	CSVClient& operator=( const CSVClient& ) = delete;
};

#endif //CSVCLIENT_H