#ifndef ENGINE_CLIENT_CCLIENT_H
#define ENGINE_CLIENT_CCLIENT_H

#include <enet/forward.h>

#include <google/protobuf/forward.h>

#include "CCLServer.h"

#include "lib/LibInterface.h"

class IGameClientInterface;
class CCommand;

/**
*	@defgroup EngineClient Engine Client systems.
*
*	@{
*/

/**
*	The client's representation of itself.
*/
class CClient final
{
public:
	/**
	*	Constructor.
	*/
	CClient();

	/**
	*	Destructor.
	*/
	~CClient();

	/**
	*	Connects the server with any systems it needs access to.
	*	@param factories List of factories to use when connecting.
	*	@param uiNumFactories Number of factories.
	*	@return true on success, false otherwise.
	*/
	bool Connect( const CreateInterfaceFn* factories, const size_t uiNumFactories );

	/**
	*	@return Whether this client is initialized.
	*/
	bool IsInitialized() const { return m_bInitialized; }

	/**
	*	@return The client's network string table manager.
	*/
	CNetworkStringTableManager& GetNetStringTableManager() { return m_Server.GetNetStringTableManager(); }

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
	*	@return Whether the client is connected to a server.
	*/
	bool IsConnected() const { return m_Server.IsConnected(); }

	/**
	*	@return Whether the client is fully connected to a server.
	*/
	bool IsFullyConnected() const { return m_Server.IsFullyConnected(); }

	/**
	*	Connects to a server.
	*	@param pszAddress IP address to connect to.
	*	@param port Port to connect to.
	*	@return true on success, false otherwise.
	*/
	bool ConnectToServer( const char* pszAddress, const enet_uint16 port );

	/**
	*	Called by the server when the client has connected to it.
	*/
	void Connected();

	/**
	*	If connected to a server, disconnects from the server.
	*	@param disconnectCode Disconnect code to provide.
	*/
	void DisconnectFromServer( const CLDisconnectCode::CLDisconnectCode disconnectCode = CLDisconnectCode ::USER_DISCONNECTED );

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

	/**
	*	Called when a client has entered a command.
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
	*	Processes a single packet from a single server.
	*	@param server Server whose packet this is.
	*	@param pPacket Packet to process.
	*/
	void ProcessPacket( CCLServer& server, ENetPacket* pPacket );

	/**
	*	Dispatches messages to the server.
	*/
	void DispatchServerMessages();

private:
	bool m_bInitialized = false;

	ENetHost* m_pHost = nullptr;

	/**
	*	Server the client is currently connected to, if any.
	*/
	CCLServer m_Server;

	IGameClientInterface* m_pGameClient = nullptr;

private:
	CClient( const CClient& ) = delete;
	CClient& operator=( const CClient& ) = delete;
};

/** @} */

#endif //ENGINE_CLIENT_CCLIENT_H