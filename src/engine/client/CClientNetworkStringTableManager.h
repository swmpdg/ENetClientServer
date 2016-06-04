#ifndef CCLIENTNETWORKSTRINGTABLEMANAGER_H
#define CCLIENTNETWORKSTRINGTABLEMANAGER_H

#include "networking/stringtable/CNetworkStringTableManager.h"

class IGameClientInterface;

namespace sv_cl_messages
{
class NetTables;
class NetTable;
}

/**
*	@ingroup EngineClient
*
*	@{
*/

/**
*	Client network string table manager. Handles message processing for table deserialization.
*/
class CClientNetworkStringTableManager final : public CNetworkStringTableManager
{
public:

	/**
	*	Sets the game client interface.
	*/
	void SetGameClient( IGameClientInterface* const pGameClient );

	/**
	*	Process a net tables message. This will create the tables listed in the message.
	*/
	void ProcessNetTablesMessage( const sv_cl_messages::NetTables& tables );

	/**
	*	Process a net table message. This will update an existing table.
	*/
	void ProcessNetTableMessage( sv_cl_messages::NetTable& table );

private:
	IGameClientInterface* m_pGameClient = nullptr;
};

/** @} */

#endif //CCLIENTNETWORKSTRINGTABLEMANAGER_H