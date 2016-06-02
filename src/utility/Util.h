#ifndef UTILITY_UTIL_H
#define UTILITY_UTIL_H

#include "CHashStringPool.h"

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

#endif //UTILITY_UTIL_H