#ifndef NETWORKING_NETWORKCONSTANTS_H
#define NETWORKING_NETWORKCONSTANTS_H

#include <cstdint>

#include <enet/types.h>

/**
*	@ingroup Networking
*
*	@{
*/

namespace NetChannel
{
/**
*	Available channels to send over.
*/
enum NetChannel : size_t
{
	/**
	*	Channel used to send game data and info.
	*/
	DATA = 0,

	/**
	*	Channel used to send files.
	*/
	FILE = 1,

	/**
	*	Number of channels. Not a valid channel.
	*/
	COUNT
};
}

namespace CLDisconnectCode
{
/**
*	Codes passed to enet_peer_disconnect for client->server
*/
enum CLDisconnectCode : enet_uint32
{
	/**
	*	Generic disconnect.
	*/
	GENERIC = 0,

	/**
	*	The user disconnected from the server.
	*/
	USER_DISCONNECTED,

	/**
	*	A bad message was received.
	*/
	BAD_MESSAGE,
};
}

namespace SVDisconnectCode
{
/**
*	Codes passed to enet_peer_disconnect for server->client
*/
enum SVDisconnectCode : enet_uint32
{
	/**
	*	Generic disconnect.
	*/
	GENERIC = 0,

	/**
	*	No free slots on the server (ENet should handle this for us)
	*/
	NO_FREE_SLOTS,

	/**
	*	Connection rejected by server.
	*/
	CONNECTION_REJECTED,

	/**
	*	Reliable channel overflowed.
	*/
	RELIABLE_CHANNEL_OVERFLOW,
};

/**
*	@return A string representation of the disconnect code.
*/
const char* ToString( const SVDisconnectCode code );
}

/**
*	Connection stages for clients connecting to the server.
*/
enum class ClientConnStage : uint8_t
{
	/**
	*	The client is not in the connecting state.
	*/
	NONE = 0,

	/**
	*	Currently receiving server info.
	*/
	SERVER_INFO,

	/**
	*	Currently receiving network string tables
	*/
	NETTABLES,
};

/**
*	Client to Server messages.
*/
enum class CLSVMessage : uint8_t
{
	/**
	*	Maps to the first message ID.
	*/
	_FIRST = 0,

	/**
	*	End of messages.
	*/
	NONE = _FIRST,

	/**
	*	Does nothing, just keeps connection alive.
	*/
	NOOP,

	/**
	*	Executes a client command.
	*/
	CLIENTCMD,

	/**
	*	A connection command. Used to indicate successful receipt of data.
	*/
	CONNECTIONCMD,

	/**
	*	Maps to the last message ID.
	*/
	_LAST = CONNECTIONCMD,

	/**
	*	Number of messages. Must be last.
	*/
	_COUNT
};

/**
*	@return A string representation of a client-to-server message ID.
*/
const char* CLSVMessageToString( const CLSVMessage message );

/**
*	Server to Client messages.
*/
enum class SVCLMessage : uint8_t
{
	/**
	*	Maps to the first message ID.
	*/
	_FIRST = 0,

	/**
	*	End of messages.
	*/
	NONE = _FIRST,

	/**
	*	Does nothing, just keeps connection alive.
	*/
	NOOP,

	/**
	*	Message to be printed.
	*/
	CLIENTPRINT,

	/**
	*	Server info message.
	*/
	SERVERINFO,

	/**
	*	Net tables creation message.
	*/
	NETTABLES,

	/**
	*	Network string table message.
	*/
	NETTABLE,

	/**
	*	Informs the client that they are fully connected.
	*/
	FULLYCONNECTED,

	/**
	*	Message explaining disconnect.
	*/
	DISCONNECT,

	/**
	*	Maps to the last message ID.
	*/
	_LAST = DISCONNECT,

	/**
	*	Number of messages. Must be last.
	*/
	_COUNT
};

/**
*	@return A string representation of a server-to-client message ID.
*/
const char* SVCLMessageToString( const SVCLMessage message );

/**
*	Maximum amount of data that can be stored in a buffer at any point in time.
*/
const size_t MAX_DATAGRAM = 80000;

/**
*	Number of bits used to represent the size parameter of a network message.
*/
const size_t NETMSG_SIZE_BITS = 32;

/**
*	ProcessMessage result types.
*/
enum class ProcessResult : uint8_t
{
	/**
	*	Message successfully processed.
	*/
	SUCCESS = 0,

	/**
	*	Deserialization failure.
	*/
	DESERIALIZE_FAILURE,

	/**
	*	An unknown message was encountered.
	*/
	UNKNOWN_MESSAGE,

	/**
	*	Message successfully deserialized, but exit condition reached.
	*/
	EXIT
};

/** @} */

#endif //NETWORKING_NETWORKCONSTANTS_H