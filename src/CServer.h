#ifndef CSERVER_H
#define CSERVER_H

#include <enet/enet.h>

#include <google/protobuf/message.h>

#include "NetworkConstants.h"

#include "utility/CNetworkStringTableManager.h"
#include "CServerNetworkStringTableManager.h"

//Windows
#undef SendMessage

class CSVClient;

/**
*	The server's representation of itself.
*/
class CServer final
{
public:
	/**
	*	Constructor.
	*/
	CServer();

	/**
	*	Destructor.
	*/
	~CServer();

	/**
	*	@return Whether the server is initialized.
	*/
	bool IsInitialized() const { return m_bInitialized; }

	/**
	*	@return Array of clients.
	*/
	CSVClient* GetClients() { return m_pClients; }

	/**
	*	@return The maximum number of clients that can connect to this server.
	*/
	size_t GetMaxClients() const { return m_uiMaxClients; }

	/**
	*	@return The server's network string table manager.
	*/
	CNetworkStringTableManager& GetNetStringTableManager() { return m_NetStringTableManager; }

	/**
	*	Called when the server first starts. Initializes the network connection and the client list with the given number of clients.
	*	@param uiMaxClients Maximum number of clients to support.
	*	@param uiPort Port to listen on.
	*	@return true if initialization succeeded, false otherwise.
	*/
	bool Initialize( const size_t uiMaxClients, const enet_uint16 uiPort );

	/**
	*	Shuts down the server. Disconnects all clients, clears all memory and closes the network connection.
	*/
	void Shutdown();

	/**
	*	Runs a single server frame.
	*/
	void RunFrame();

	/**
	*	Sends a message to all clients.
	*	@param messageId Message ID.
	*	@param message Message to send.
	*	@return true on success, false otherwise.
	*/
	bool SendBroadcastMessage( const SVCLMessage messageId, google::protobuf::Message& message );

private:

	/**
	*	Processes network events.
	*/
	void ProcessNetworkEvents();

	/**
	*	Processes a single packet from a single client.
	*	@param client Client whose packet this is.
	*	@param pPacket Packet to process.
	*/
	void ProcessPacket( CSVClient& client, ENetPacket* pPacket );

	/**
	*	Send network string tables to all connected clients.
	*/
	void SendNetTables();

	/**
	*	Dispatch pending messages to all clients.
	*/
	void DispatchClientMessages();

private:
	bool m_bInitialized = false;

	/**
	*	Network host.
	*/
	ENetHost* m_pHost = nullptr;

	/**
	*	List of clients.
	*/
	CSVClient* m_pClients = nullptr;

	/**
	*	Maximum number of clients.
	*/
	size_t m_uiMaxClients = 0;

	CServerNetworkStringTableManager m_NetStringTableManager;

private:
	CServer( const CServer& ) = delete;
	CServer& operator=( const CServer& ) = delete;
};

#endif //CSERVER_H