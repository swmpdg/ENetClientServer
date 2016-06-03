#ifndef UTILITY_CNETWORKSTRINGTABLEMANAGER_H
#define UTILITY_CNETWORKSTRINGTABLEMANAGER_H

#include <vector>

#include "networking/shared/stringtable/INetworkStringTableManager.h"
#include "CNetworkStringTable.h"

class CNetworkBuffer;

/**
*	Manages the list of network string tables.
*/
class CNetworkStringTableManager : public INetworkStringTableManager
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

	size_t GetNumTables() const override final { return m_Tables.size(); }

	const CNetworkStringTable* GetTableByIndex( const size_t uiIndex ) const override final;

	CNetworkStringTable* GetTableByIndex( const size_t uiIndex ) override final;

	const CNetworkStringTable* GetTableByName( const char* const pszName ) const override final;

	CNetworkStringTable* GetTableByName( const char* const pszName ) override final;

	CNetworkStringTable* CreateTable( const char* const pszName ) override final;

	/**
	*	@return Whether table creation is allowed or not.
	*/
	bool TableCreationAllowed() const { return m_bAllowTableCreation; }

	/**
	*	Sets whether table creation is allowed or not.
	*/
	void SetAllowTableCreation( const bool bAllowTableCreation )
	{
		m_bAllowTableCreation = bAllowTableCreation;
	}

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
	bool UnserializeTable( const NST::TableID_t ID, CNetworkBuffer& buffer );

protected:
	Tables_t m_Tables;

	bool m_bAllowTableCreation = false;

private:
	CNetworkStringTableManager( const CNetworkStringTableManager& ) = delete;
	CNetworkStringTableManager& operator=( const CNetworkStringTableManager& ) = delete;
};

#endif //UTILITY_CNETWORKSTRINGTABLEMANAGER_H