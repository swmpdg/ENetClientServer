#ifndef UTILITY_NETWORKUTILS_H
#define UTILITY_NETWORKUTILS_H

#include <google/protobuf/message.h>

class CNetworkBuffer;

/**
*	Serializes the given message to the given buffer.
*	@param iMessageId Message ID.
*	@param message Message to serialize.
*	@param buffer Buffer to serialize to.
*	@return true on success, false on failure.
*/
bool SerializeToBuffer( const int iMessageId, const google::protobuf::Message& message, CNetworkBuffer& buffer );

template<typename T>
bool SerializeToBuffer( const T iMessageId, const google::protobuf::Message& message, CNetworkBuffer& buffer )
{
	return SerializeToBuffer( static_cast<int>( iMessageId ), message, buffer );
}

#endif //UTILITY_NETWORKUTILS_H