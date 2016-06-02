#ifndef CGAME_H
#define CGAME_H

#include <atomic>
#include <mutex>
#include <string>

#include <enet/enet.h>

#include "engine/server/CServer.h"
#include "engine/client/CClient.h"

class CNetworkStringTable;

/**
*	The game's representation of itself.
*/
class CGame final
{
public:
	/**
	*	Constructor.
	*/
	CGame() = default;

	/**
	*	Destructor.
	*/
	~CGame() = default;

	/**
	*	Initializes the game.
	*	@param uiPort The port that the server listens on.
	*	@return true on success, false otherwise.
	*/
	bool Initialize( const enet_uint16 uiPort );

	/**
	*	Shuts down the game. Must be called even if Initialize returned false.
	*/
	void Shutdown();

	/**
	*	Runs the game loop.
	*	@return true on success, false otherwise.
	*/
	bool Run();

private:
	/**
	*	Entry point for the IO thread.
	*	@param pGame Game instance.
	*/
	static void IOThread( CGame* pGame );

	/**
	*	IO thread main loop.
	*/
	void RunIO();

private:
	CServer m_Server;
	CClient m_Client;

	enet_uint16 m_uiPort;

	std::atomic<bool> m_bTerminate = false;
	std::mutex m_IOMutex;

	std::string m_szInput;

	bool m_bInputPending = false;

	CNetworkStringTable* m_pServerTable = nullptr;
	CNetworkStringTable* m_pClientTable = nullptr;

private:
	CGame( const CGame& ) = delete;
	CGame& operator=( const CGame& ) = delete;
};

#endif //CGAME_H