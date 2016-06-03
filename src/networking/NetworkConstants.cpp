#include "NetworkConstants.h"

namespace SVDisconnectCode
{
const char* ToString( const SVDisconnectCode code )
{
	switch( code )
	{
	default:
	case GENERIC:						return "Disconnected from server";
	case NO_FREE_SLOTS:					return "No free slots";
	case CONNECTION_REJECTED:			return "Connection rejected";
	case RELIABLE_CHANNEL_OVERFLOW:		return "Reliable channel overflow";
	}
}
}