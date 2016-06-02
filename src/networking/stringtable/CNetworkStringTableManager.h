#ifndef UTILITY_CNETWORKSTRINGTABLEMANAGER_H
#define UTILITY_CNETWORKSTRINGTABLEMANAGER_H

#include <vector>

#include "networking/stringtable/CNetworkStringTable.h"

class CNetworkBuffer;

/**
*	Manages the list of network string tables.
*/
class CNetworkStringTableManager
{
private:
	typedef std::vector<CNetworkStringTable*> Tables_t;

public:
	/**
	*	Constructor.
	*/
	CNetworkStringTableManager() = default;

	/**
	*	Destructor.
	*/
	~CNetworkStringTableManager() = default;

	/**
	*	@return The number of tables.
	*/
	size_t GetNumTables() const { return m_Tables.size(); }

	/**
	*	Gets a table by index.
	*	@return Table, or null if no table at that index exists.
	*/
	const CNetworkStringTable* GetTableByIndex( const size_t uiIndex ) const;

	/**
	*	@copydoc GetTable( const size_t uiIndex ) const
	*/
	CNetworkStringTable* GetTableByIndex( const size_t uiIndex );

	/**
	*	Gets a table by name.
	*	@return Table, or null if no table at that index exists.
	*/
	const CNetworkStringTable* GetTableByName( const char* const pszName ) const;

	/**
	*	@copydoc GetTableByName( const char* const pszName ) const
	*/
	CNetworkStringTable* GetTableByName( const char* const pszName );

	/**
	*	Creates a new table. If a table with the given name already exists, returns null.
	*	@param pszName Name of the table. Must be a valid, non-empty name that uniquely identifies this table.
	*	@return Table, or null if the given parameters are invalid.
	*/
	virtual CNetworkStringTable* CreateTable( const char* const pszName ) = 0;

	/**
	*	Clears all tables.
	*/
	void Clear();

	/**
	*	Serializes all changes in all tables to the given buffer.
	*	@return true if data was sent, false otherwise.
	*/
	bool Serialize( CNetworkBuffer& buffer, const float flTime = -1 );

	/**
	*	Unserializes table data from the given buffer.
	*	@return true on success, false otherwise.
	*/
	bool UnserializeTable( const CNetworkStringTable::TableID_t ID, CNetworkBuffer& buffer );

protected:
	Tables_t m_Tables;

private:
	CNetworkStringTableManager( const CNetworkStringTableManager& ) = delete;
	CNetworkStringTableManager& operator=( const CNetworkStringTableManager& ) = delete;
};

#endif //UTILITY_CNETWORKSTRINGTABLEMANAGER_H