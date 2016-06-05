#include <cassert>

#include "utility/Util.h"

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

namespace
{
static const char* g_pszCLSVMessageNames[ static_cast<size_t>( CLSVMessage::_COUNT ) ] =
{
	STRINGIZE( CLSVMessage::NONE ),
	STRINGIZE( CLSVMessage::SERVER_INFO ),
	STRINGIZE( CLSVMessage::NETTABLES ),
};
}

const char* CLSVMessageToString( const CLSVMessage message )
{
	assert( CLSVMessage::_FIRST <= message && message <= CLSVMessage::_LAST );

	return g_pszCLSVMessageNames[ static_cast<size_t>( message ) ];
}

namespace
{
static const char* g_pszSVCLMessageNames[ static_cast<size_t>( SVCLMessage::_COUNT ) ] = 
{
	STRINGIZE( SVCLMessage::NONE ),
	STRINGIZE( SVCLMessage::NOOP ),
	STRINGIZE( SVCLMessage::CLIENTPRINT ),
	STRINGIZE( SVCLMessage::SERVERINFO ),
	STRINGIZE( SVCLMessage::NETTABLES ),
	STRINGIZE( SVCLMessage::NETTABLE ),
	STRINGIZE( SVCLMessage::FULLYCONNECTED ),
	STRINGIZE( SVCLMessage::DISCONNECT ),
};
}

const char* SVCLMessageToString( const SVCLMessage message )
{
	assert(  SVCLMessage::_FIRST <= message && message <= SVCLMessage::_LAST );

	return g_pszSVCLMessageNames[ static_cast<size_t>( message ) ];
}