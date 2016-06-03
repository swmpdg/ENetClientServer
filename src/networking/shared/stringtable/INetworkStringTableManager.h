#ifndef NETWORKING_STRINGTABLE_INETWORKSTRINGTABLEMANAGER_H
#define NETWORKING_STRINGTABLE_INETWORKSTRINGTABLEMANAGER_H

#include "NetStringTableConstants.h"

class INetworkStringTable;

/**
*	Manages the list of network string tables.
*/
class INetworkStringTableManager
{
public:
	virtual ~INetworkStringTableManager() = 0;

	/**
	*	@return The number of tables.
	*/
	virtual size_t GetNumTables() const = 0;

	/**
	*	Gets a table by index.
	*	@return Table, or null if no table at that index exists.
	*/
	virtual const INetworkStringTable* GetTableByIndex( const size_t uiIndex ) const = 0;

	/**
	*	@copydoc GetTable( const size_t uiIndex ) const
	*/
	virtual INetworkStringTable* GetTableByIndex( const size_t uiIndex ) = 0;

	/**
	*	Gets a table by name.
	*	@return Table, or null if no table at that index exists.
	*/
	virtual const INetworkStringTable* GetTableByName( const char* const pszName ) const = 0;

	/**
	*	@copydoc GetTableByName( const char* const pszName ) const
	*/
	virtual INetworkStringTable* GetTableByName( const char* const pszName ) = 0;

	/**
	*	Creates a new table. If a table with the given name already exists, returns null. Table creation is restricted to certain points in time.
	*	@param pszName Name of the table. Must be a valid, non-empty name that uniquely identifies this table.
	*	@return Table, or null if the given parameters are invalid.
	*/
	virtual INetworkStringTable* CreateTable( const char* const pszName ) = 0;
};

inline INetworkStringTableManager::~INetworkStringTableManager()
{
}

#endif //NETWORKING_STRINGTABLE_INETWORKSTRINGTABLEMANAGER_H