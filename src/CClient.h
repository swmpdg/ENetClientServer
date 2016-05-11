#ifndef CCLIENT_H
#define CCLIENT_H

#include <enet/enet.h>

#include <google/protobuf/message.h>

#include "CCLServer.h"

//Windows define
#undef SendMessage

/**
*	The client's representation of itself.
*/
class CClient final
{
public:
	/**
	*	Constructor.
	*/
	CClient() = default;

	/**
	*	Destructor.
	*/
	~CClient();

	/**
	*	@return Whether this client is initializated.
	*/
	bool IsInitialized() const { return m_bInitialized; }

	/**
	*	Called when the client first starts. Initializes the network connection.
	*	@return true on success, false otherwise.
	*/
	bool Initialize();

	/**
	*	Shuts down the client. If connected to a server, disconnects, clears all memory and closes the network connection.
	*/
	void Shutdown();

	/**
	*	Returns whether the client is connected to a server.
	*/
	bool IsConnected() const { return m_Server.IsConnected(); }

	/**
	*	Connects to a server.
	*	@param pszAddress IP address to connect to.
	*	@param port Port to connect to.
	*	@return true on success, false otherwise.
	*/
	bool ConnectToServer( const char* pszAddress, const enet_uint16 port );

	/**
	*	If connected to a server, disconnects from the server.
	*/
	void DisconnectFromServer();

	/**
	*	Sends a message to the server.
	*	@param messageId Message ID.
	*	@param message Message to send.
	*	@return true on success, false otherwise.
	*/
	bool SendMessage( const CLSVMessage messageId, google::protobuf::Message& message );

	/**
	*	Runs a single client frame.
	*/
	void RunFrame();

private:

	/**
	*	Processes network events.
	*/
	void ProcessNetworkEvents();

	/**
	*	Processes a single packet from a single server.
	*	@param server Server whose packet this is.
	*	@param pPacket Packet to process.
	*/
	void ProcessPacket( CCLServer& server, ENetPacket* pPacket );

private:
	bool m_bInitialized = false;

	ENetHost* m_pHost = nullptr;

	/**
	*	Server the client is currently connected to, if any.
	*/
	CCLServer m_Server;

private:
	CClient( const CClient& ) = delete;
	CClient& operator=( const CClient& ) = delete;
};

#endif //CCLIENT_H