#ifndef UTILITY_CHASHSTRINGPOOL_H
#define UTILITY_CHASHSTRINGPOOL_H

#include <unordered_set>

#include "CAutoString.h"

/**
*	A string pool that stores strings in a hash map. Efficiently manages the given strings, ensuring no copying occurs.
*/
class CHashStringPool final
{
private:
	typedef std::unordered_set<CAutoString> Strings_t;

public:
	/**
	*	Constructor.
	*/
	CHashStringPool();

	/**
	*	Destructor.
	*/
	~CHashStringPool() = default;

	/**
	*	Finds the given string.
	*	@param pszString String to find.
	*	@param pszDefault Default string to return.
	*	@return Pointer to the given string that is managed by this string pool. If the string is not found, returns pszDefault.
	*/
	const char* Find( const char* pszString, const char* const pszDefault = "" ) const;

	/**
	*	Adds the given string to the pool.
	*	@param pszString String to add.
	*	@param pNewAllocation If not null, contains whether the string was newly allocated or already present in the pool.
	*	@return Pointer to the string that was inserted in the pool.
	*/
	const char* Allocate( const char* pszString, bool* pNewAllocation = nullptr );

	/**
	*	Clears the entire pool.
	*/
	void Clear();

	/**
	*	Prints debug info to stdout.
	*/
	void DebugPrint() const;

private:
	Strings_t m_Strings;

private:
	CHashStringPool( const CHashStringPool& ) = delete;
	CHashStringPool& operator=( const CHashStringPool& ) = delete;
};

#endif //UTILITY_CHASHSTRINGPOOL_H