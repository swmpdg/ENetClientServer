#ifndef ENGINE_CENGINE_H
#define ENGINE_CENGINE_H

#include <atomic>
#include <mutex>
#include <string>

#include <enet/types.h>

#include "engine/server/CServer.h"
#include "engine/client/CClient.h"

/**
*	@defgroup Engine Engine systems
*
*	@{
*/

/**
*	The engine's representation of itself.
*/
class CEngine final
{
public:
	/**
	*	Constructor.
	*/
	CEngine() = default;

	/**
	*	Destructor.
	*/
	~CEngine() = default;

	/**
	*	Initializes the engine.
	*	@param uiPort The port that the server listens on.
	*	@return true on success, false otherwise.
	*/
	bool Initialize( const enet_uint16 uiPort );

	/**
	*	Shuts down the engine. Must be called even if Initialize returned false.
	*/
	void Shutdown();

	/**
	*	Runs the engine loop.
	*	@return true on success, false otherwise.
	*/
	bool Run();

private:
	/**
	*	Runs the main engine game loop.
	*/
	bool RunGameLoop();

	/**
	*	Entry point for the IO thread.
	*	@param pEngine Engine instance.
	*/
	static void IOThread( CEngine* pEngine );

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

private:
	CEngine( const CEngine& ) = delete;
	CEngine& operator=( const CEngine& ) = delete;
};

/** @} */

#endif //ENGINE_CENGINE_H