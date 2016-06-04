#ifndef UTILITY_UTIL_H
#define UTILITY_UTIL_H

#include "CHashStringPool.h"

/**
*	@defgroup Utility Utility functionality.
*
*	@{
*/

/**
*	Global string pool.
*/
extern CHashStringPool g_StringPool;

/**
*	Returns the current tick time, in milliseconds.
*	@return Tick time, in milliseconds.
*/
long long GetCurrentTick();

/**
*	Gets the current time, in seconds.
*	@return Current time, in seconds.
*/
double GetCurrentTime();

/**
*	Finds the number of characters the given strings have in common.
*	@param pszString1 First string to compare.
*	@param pszString2 Second string to compare.
*	@return Number of common characters.
*/
size_t UTIL_FindCommonBaseCount( const char* pszString1, const char* pszString2 );

/** @} */

#endif //UTILITY_UTIL_H