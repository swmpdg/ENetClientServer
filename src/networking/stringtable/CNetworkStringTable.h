#ifndef UTILITY_CNETWORKSTRINGTABLE_H
#define UTILITY_CNETWORKSTRINGTABLE_H

#include <vector>

#include "networking/shared/stringtable/INetworkStringTable.h"

#include "PrivateNetStringTableConstants.h"

class CNetworkBuffer;

/**
*	A table of strings that can be networked between the server and all clients.
*/
class CNetworkStringTable final : public INetworkStringTable
{
private:
	/**
	*	Represents a single entry in the table.
	*/
	struct TableEntry_t
	{
		/**
		*	Pointer to externally managed memory. Lifetime matches the map's.
		*/
		const char* pszString = nullptr;

		//Last time this entry was modified.
		float flLastModifiedTime = 0;
	};

	typedef std::vector<TableEntry_t> TableEntries_t;

	/**
	*	The number of bits to use to represent common string sequences.
	*/
	static const size_t NUM_COMMON_BITS = 5;
	static const size_t COMMON_BITS_MASK = ( 1 << NUM_COMMON_BITS ) - 1;

public:
	/**
	*	Constructor.
	*/
	CNetworkStringTable( const char* const pszName, const NST::TableID_t tableID, const size_t uiMaxEntries );

	/**
	*	Destructor.
	*/
	~CNetworkStringTable() = default;

	const char* GetName() const override final { return m_pszName; }

	NST::TableID_t GetID() const override final { return m_TableID; }

	size_t GetMaxEntries() const override final { return m_uiMaxEntries; }

	/**
	*	@return The number of strings that are in this table.
	*/
	size_t GetStringCount() const override final { return m_TableEntries.size(); }

	/**
	*	Gets the index of the given string.
	*	@param pszString String to find.
	*	@return Index of the string, or INVALID_INDEX if not found.
	*/
	size_t IndexOf( const char* const pszString ) const override final;

	/**
	*	@return Whether the given index is valid or not.
	*/
	bool IsValidIndex( const size_t uiIndex ) const override final;

	/**
	*	@return String at the given index, or null if the index is invalid.
	*/
	const char* GetString( const size_t uiIndex ) const override final;

	/**
	*	Adds a string to the table. If the string is already in the table, does nothing.
	*	@return Index of the string.
	*/
	size_t Add( const char* const pszString ) override final;

	/**
	*	Clears the table.
	*/
	void Clear();

	/**
	*	@return The last time this table was modified.
	*/
	float GetLastModifiedTime() const { return m_flLastModifiedTime; }

	/**
	*	Serializes the table to the given buffer.
	*	@param buffer Buffer to serialize to.
	*	@param flTime Used for change detection.
	*	@param uiStart Index to start at.
	*	@return true if any data was written, false otherwise.
	*/
	NST::SerializeResult Serialize( CNetworkBuffer& buffer, const float flTime, const size_t uiStart = 0, const bool bAllowOverflow = false );

	/**
	*	Unserializes the given buffer into this table.
	*	@return true on success, false otherwise.
	*/
	bool Unserialize( CNetworkBuffer& buffer );

private:
	const char* const m_pszName;
	const NST::TableID_t m_TableID;

	const size_t m_uiMaxEntries;

	//Bit mask for entry index
	const size_t m_uiEntriesBits;

	TableEntries_t m_TableEntries;

	//Last time the table was modified.
	float m_flLastModifiedTime = 0;

private:
	CNetworkStringTable( const CNetworkStringTable& ) = delete;
	CNetworkStringTable& operator=( const CNetworkStringTable& ) = delete;
};

#endif //UTILITY_CNETWORKSTRINGTABLE_H