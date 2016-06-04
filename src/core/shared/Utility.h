#ifndef CORE_UTILITY_H
#define CORE_UTILITY_H

#include <algorithm>
#include <cstdint>

#include "Platform.h"

/**
*	Utility functionality.
*/

/**
*	Sizeof for array types. Only works for arrays with a known size (stack buffers).
*	@tparam T Array type. Automatically inferred.
*	@tparam SIZE Number of elements in the array.
*	@return Number of elements in the array.
*/
template<typename T, const size_t SIZE>
constexpr inline size_t _ArraySizeof( T( &)[ SIZE ] )
{
	return SIZE;
}

/**
*	Replaces ARRAYSIZE. ARRAYSIZE is defined in some platform specific headers.
*/
#define ARRAYSIZE _ArraySizeof

#define MAX_BUFFER_LENGTH 512

#endif //CORE_UTILITY_H
