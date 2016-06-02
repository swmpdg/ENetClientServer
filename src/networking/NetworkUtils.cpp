#include "NetworkConstants.h"

#include "CNetworkBuffer.h"

#include "CNetBufOutputStream.h"

#include "NetworkUtils.h"

bool SerializeToBuffer( const int iMessageId, const google::protobuf::Message& message, CNetworkBuffer& buffer )
{
	buffer.WriteByte( iMessageId );

	const size_t messageSize = message.ByteSize();

	buffer.WriteUnsignedBitLong( messageSize, NETMSG_SIZE_BITS );

	CNetBufOutputStream stream( buffer );

	if( !message.SerializeToZeroCopyStream( &stream ) )
	{
		printf( "Failed to serialize message\n" );
		return false;
	}

	if( buffer.HasOverflowed() )
	{
		printf( "Message buffer overflowed\n" );
		return false;
	}

	return true;
}