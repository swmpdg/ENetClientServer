#ifndef ENGINE_SERVER_CSERVER_H
#define ENGINE_SERVER_CSERVER_H

#include <enet/forward.h>

#include <google/protobuf/forward.h>

#include "networking/NetworkConstants.h"

#include "networking/stringtable/CNetworkStringTableManager.h"
#include "CServerNetworkStringTableManager.h"

#include "lib/LibInterface.h"

class CSVClient;

class IGameServerInterface;

class CCommand;

/**
*	@defgroup EngineServer Engine Server systems.
*
*	@{
*/

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

	ENetHost* GetHost() { return m_pHost; }

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
	CServerNetworkStringTableManager& GetNetStringTableManager() { return m_NetStringTableManager; }

	/**
	*	@return Game server interface.
	*/
	IGameServerInterface* GetGameServer() { return m_pGameServer; }

	/**
	*	Connects the server with any systems it needs access to.
	*	@param factories List of factories to use when connecting.
	*	@param uiNumFactories Number of factories.
	*	@return true on success, false otherwise.
	*/
	bool Connect( const CreateInterfaceFn* factories, const size_t uiNumFactories );

	/**
	*	Called when the server first starts. Initializes the network connection and the client list with the given number of clients.
	*	@param uiMaxClients Maximum number of clients to support.
	*	@param uiPort Port to listen on.
	*	@return true if initialization succeeded, false otherwise.
	*/
	bool Initialize( const size_t uiMaxClients, const enet_uint16 uiPort );

	/**
	*	Changes the server to the given map.
	*/
	void ChangeLevel( const char* const pszMapName );

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

	/**
	*	Called when a client has entered a command.
	*	TODO: needs an object that represents the client.
	*	@param command Object that contains command arguments.
	*	@return true if handled, false otherwise.
	*/
	bool ClientCommand( const CCommand& command );

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

	IGameServerInterface* m_pGameServer = nullptr;

private:
	CServer( const CServer& ) = delete;
	CServer& operator=( const CServer& ) = delete;
};

/** @} */

#endif //ENGINE_SERVER_CSERVER_H