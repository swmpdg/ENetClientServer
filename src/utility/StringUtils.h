#ifndef UTILITY_STRINGUTILS_H
#define UTILITY_STRINGUTILS_H

#include <cstdlib>
#include <cstring>

/**
*	@ingroup Utility
*
*	@{
*/

/*
* String hashing function
* Ripped from the MSVC std::hash<std::string> implementation
*/
inline std::size_t StringHash( const char* pszString )
{
	if( !pszString )
		pszString = "";

	size_t _Val = 2166136261U;
	size_t _First = 0;
	size_t _Last = strlen( pszString );
	size_t _Stride = 1 + _Last / 10;

	for( ; _First < _Last; _First += _Stride )
		_Val = 16777619U * _Val ^ ( size_t ) pszString[ _First ];
	return ( _Val );
}

/** @} */

#endif //UTILITY_STRINGUTILS_H