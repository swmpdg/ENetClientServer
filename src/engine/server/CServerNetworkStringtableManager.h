#ifndef ENGINE_SERVER_CSERVERNETWORKSTRINGTABLEMANAGER_H
#define ENGINE_SERVER_CSERVERNETWORKSTRINGTABLEMANAGER_H

#include "networking/stringtable/CNetworkStringTableManager.h"

class CServer;

class CNetworkBuffer;

/**
*	@ingroup EngineServer
*
*	@{
*/

/**
*	Server network string table manager. Handles message creation for table serialization.
*/
class CServerNetworkStringTableManager final : public CNetworkStringTableManager
{
public:
	/**
	*	Constructor.
	*	@param pServer Server instance that this manager belongs to.
	*/
	CServerNetworkStringTableManager( CServer* const pServer );

	/**
	*	Write create messages for each table into the buffer.
	*/
	void WriteNetTableCreateMessages( CNetworkBuffer& buffer );

	/**
	*	Writes a baseline to send to clients at connect time.
	*	@param uiTableIndex The table to write a baseline for.
	*	@param uiStringIndex First string in the table to process.
	*	@param flTime Timebase used to determine which strings should be written.
	*	@param buffer Buffer to write the baseline to.
	*	@return Serialization result.
	*/
	NST::SerializeResult WriteBaseline( const size_t uiTableIndex, const size_t uiStringIndex, const float flTime, CNetworkBuffer& buffer );

private:
	CServer* const m_pServer;
};

/** @} */

#endif //ENGINE_SERVER_CSERVERNETWORKSTRINGTABLEMANAGER_H