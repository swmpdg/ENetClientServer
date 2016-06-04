#ifndef UTILITY_CNETWORKSTRINGTABLE_H
#define UTILITY_CNETWORKSTRINGTABLE_H

#include <vector>

#include "networking/shared/stringtable/INetworkStringTable.h"

#include "PrivateNetStringTableConstants.h"

class CNetworkBuffer;

/**
*	@ingroup NetStringTable
*
*	@{
*/

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
		*	Pointer to externally managed memory. Lifetime must match or exceed the map's.
		*/
		const char* pszString = nullptr;

		//Last time this entry was modified.
		float flLastModifiedTime = 0;
	};

	typedef std::vector<TableEntry_t> TableEntries_t;

	/**
	*	The number of bits to use to represent common string sequence lengths.
	*/
	static const size_t NUM_COMMON_BITS = 5;

	/**
	*	Bit mask to use for common string sequence lengths.
	*/
	static const size_t COMMON_BITS_MASK = ( 1 << NUM_COMMON_BITS ) - 1;

public:
	/**
	*	Constructor.
	*/
	CNetworkStringTable( const char* const pszName, const NST::TableID_t tableID, const size_t uiMaxEntries );

	~CNetworkStringTable() = default;

	const char* GetName() const override final { return m_pszName; }

	NST::TableID_t GetID() const override final { return m_TableID; }

	size_t GetMaxEntries() const override final { return m_uiMaxEntries; }

	size_t GetStringCount() const override final { return m_TableEntries.size(); }

	NST::StringAddedCallback GetStringAddedCallback() const override final { return m_pStringAddedCallback; }

	void SetStringAddedCallback( NST::StringAddedCallback pCallback ) override final;

	void* GetStringAddedObject() const override final { return m_pStringAddedObject; }

	void SetStringAddedCallback( void* pObject ) override final;

	size_t IndexOf( const char* const pszString ) const override final;

	bool IsValidIndex( const size_t uiIndex ) const override final;

	const char* GetString( const size_t uiIndex ) const override final;

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
	*	@param bAllowOverflow Whether to allow overflows in the internal buffer. If the buffer provided by the buffer parameter overflows, it will still return OVERFLOWED.
	*	@return true if any data was written, false otherwise.
	*/
	NST::SerializeResult Serialize( CNetworkBuffer& buffer, const float flTime, const size_t uiStart = 0, const bool bAllowOverflow = false );

	/**
	*	Unserializes the given buffer into this table.
	*	@return true on success, false otherwise.
	*/
	bool Unserialize( CNetworkBuffer& buffer );

	/**
	*	Invokes the callback for all changes that have occurred since the given time.
	*/
	void InvokeCallbackForChanges( const float flTime );

private:
	const char* const m_pszName;
	const NST::TableID_t m_TableID;

	const size_t m_uiMaxEntries;

	//Bit mask for entry index
	const size_t m_uiEntriesBits;

	TableEntries_t m_TableEntries;

	//Last time the table was modified.
	float m_flLastModifiedTime = 0;

	NST::StringAddedCallback m_pStringAddedCallback = nullptr;
	void* m_pStringAddedObject = nullptr;

private:
	CNetworkStringTable( const CNetworkStringTable& ) = delete;
	CNetworkStringTable& operator=( const CNetworkStringTable& ) = delete;
};

/** @} */

#endif //UTILITY_CNETWORKSTRINGTABLE_H