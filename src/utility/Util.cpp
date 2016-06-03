#include <chrono>

#include "Util.h"

using namespace std::chrono;

CHashStringPool g_StringPool;

long long GetCurrentTick()
{
	return duration_cast<milliseconds>( high_resolution_clock::now().time_since_epoch() ).count();
}

double GetCurrentTime()
{
	return GetCurrentTick() / 1000.0;
}

size_t UTIL_FindCommonBaseCount( const char* pszString1, const char* pszString2 )
{
	if( !pszString1 || !( *pszString1 ) || !pszString2 || !( *pszString2 ) )
		return 0;

	size_t uiCount = 0;

	while( *pszString1 && *pszString1++ == *pszString2++ )
	{
		++uiCount;
	}

	return uiCount;
}