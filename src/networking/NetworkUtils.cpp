#include <cstdint>

#include <enet/enet.h>

#include <google/protobuf/message.h>

#include "CNetworkBuffer.h"

#include "CNetBufOutputStream.h"
#include "CNetBufInputStream.h"

#include "messages/sv_cl_messages/Disconnect.pb.h"

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

bool DeserializeFromBuffer( CNetworkBuffer& buffer, const size_t uiMessageSize, google::protobuf::Message& message )
{
	CNetBufInputStream stream( buffer, uiMessageSize );

	if( !message.ParseFromBoundedZeroCopyStream( &stream, static_cast<int>( uiMessageSize ) ) )
	{
		printf( "Failed to deserialize message!\n" );
		return false;
	}

	return true;
}

namespace NET
{
void DisconnectWithReason( ENetPeer* pPeer, const SVDisconnectCode::SVDisconnectCode disconnectCode, const char* const pszReason )
{
	assert( pPeer );

	if( pszReason && *pszReason )
	{
		uint8_t uiMsgBuf[ MAX_DATAGRAM ];

		CNetworkBuffer msgBuffer( "DisconnectWithReason_buffer", uiMsgBuf, sizeof( uiMsgBuf ) );

		sv_cl_messages::Disconnect disconnect;

		disconnect.set_reason( pszReason );

		SerializeToBuffer( SVCLMessage::DISCONNECT, disconnect, msgBuffer );

		auto pPacket = enet_packet_create( msgBuffer.GetData(), msgBuffer.GetBytesInBuffer(), ENET_PACKET_FLAG_RELIABLE );

		if( enet_peer_send( pPeer, NetChannel::DATA, pPacket ) != 0 )
		{
			printf( "DisconnectWithReason: Error while sending packet!\n" );
		}
	}

	//Disconnect later so all remaining packets are sent (i.e. the above).
	enet_peer_disconnect_later( pPeer, disconnectCode );
}
}