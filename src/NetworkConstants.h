#ifndef NETWORKCONSTANTS_H
#define NETWORKCONSTANTS_H

#include <cstdint>

#include <enet/enet.h>

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
};
}

/**
*	Client to Server messages.
*/
enum class CLSVMessage : uint8_t
{
	/**
	*	End of messages.
	*/
	NONE = 0,

	/**
	*	Does nothing, just keeps connection alive.
	*/
	NOOP,

	/**
	*	Executes a client command.
	*/
	CLIENTCMD,
};

/**
*	Server to Client messages.
*/
enum class SVCLMessage : uint8_t
{
	/**
	*	End of messages.
	*/
	NONE = 0,

	/**
	*	Does nothing, just keeps connection alive.
	*/
	NOOP,

	/**
	*	Message to be printed.
	*/
	CLIENTPRINT,
};

/**
*	Maximum amount of data that can be stored in a buffer at any point in time.
*/
const size_t MAX_DATAGRAM = 80000;

/**
*	Number of bits used to represent the size parameter of a network message.
*/
const size_t NETMSG_SIZE_BITS = 32;

#endif //NETWORKCONSTANTS_H