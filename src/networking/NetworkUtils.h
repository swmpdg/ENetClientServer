#ifndef UTILITY_NETWORKUTILS_H
#define UTILITY_NETWORKUTILS_H

#include <enet/forward.h>

#include <google/protobuf/forward.h>

#include "NetworkConstants.h"

class CNetworkBuffer;

/**
*	@ingroup Networking
*
*	@{
*/

/**
*	Serializes the given message to the given buffer.
*	@param iMessageId Message ID.
*	@param message Message to serialize.
*	@param buffer Buffer to serialize to.
*	@return true on success, false on failure.
*/
bool SerializeToBuffer( const int iMessageId, const google::protobuf::Message& message, CNetworkBuffer& buffer );

/**
*	@copydoc SerializeToBuffer( const int iMessageId, const google::protobuf::Message& message, CNetworkBuffer& buffer )
*/
template<typename T>
bool SerializeToBuffer( const T iMessageId, const google::protobuf::Message& message, CNetworkBuffer& buffer )
{
	return SerializeToBuffer( static_cast<int>( iMessageId ), message, buffer );
}

namespace NET
{
/**
*	Disconnects the given peer and sends a reason for the disconnect if provided.
*	Can be used even if there is no associated client slot for this peer.
*	@param pPeer Peer to disconnect.
*	@param disconnectCode Disconnect code to send.
*	@param pszReason If not null or empty, the reason for the disconnection to send to the peer.
*/
void DisconnectWithReason( ENetPeer* pPeer, const SVDisconnectCode::SVDisconnectCode disconnectCode, const char* const pszReason = nullptr );
}

/** @} */

#endif //UTILITY_NETWORKUTILS_H