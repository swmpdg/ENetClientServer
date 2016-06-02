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