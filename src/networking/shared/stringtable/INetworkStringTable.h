#ifndef NETWORKING_STRINGTABLE_INETWORKSTRINGTABLE_H
#define NETWORKING_STRINGTABLE_INETWORKSTRINGTABLE_H

#include "NetStringTableConstants.h"

/**
*	Represents an indexed table of strings that can be networked between a server and all connected clients.
*/
class INetworkStringTable
{
public:
	virtual ~INetworkStringTable() = 0;

	/**
	*	@return The name of this table.
	*/
	virtual const char* GetName() const = 0;

	/**
	*	@return This table's ID.
	*/
	virtual NST::TableID_t GetID() const = 0;

	/**
	*	@return Maximum number of entries.
	*/
	virtual size_t GetMaxEntries() const = 0;

	/**
	*	@return The number of strings that are in this table.
	*/
	virtual size_t GetStringCount() const = 0;

	/**
	*	Gets the index of the given string.
	*	@param pszString String to find.
	*	@return Index of the string, or INVALID_INDEX if not found.
	*/
	virtual size_t IndexOf( const char* const pszString ) const = 0;

	/**
	*	@return Whether the given index is valid or not.
	*/
	virtual bool IsValidIndex( const size_t uiIndex ) const = 0;

	/**
	*	@return String at the given index, or null if the index is invalid.
	*/
	virtual const char* GetString( const size_t uiIndex ) const = 0;

	/**
	*	Adds a string to the table. If the string is already in the table, does nothing.
	*	@return Index of the string.
	*/
	virtual size_t Add( const char* const pszString ) = 0;
};

inline INetworkStringTable::~INetworkStringTable()
{
}

#endif //NETWORKING_STRINGTABLE_INETWORKSTRINGTABLE_H