#ifndef UTILITY_CNETBUFINPUTSTREAM_H
#define UTILITY_CNETBUFINPUTSTREAM_H

#include <google/protobuf/io/zero_copy_stream.h>

#include "CNetworkBuffer.h"

/**
*	@ingroup Networking
*
*	@{
*/

/**
*	Zero copy input stream designed to interface with CNetworkBuffer.
*	@see google::protobuf::io::ZeroCopyInputStream
*	@see CNetworkBuffer
*/
class CNetBufInputStream final : public google::protobuf::io::ZeroCopyInputStream
{
public:
	/**
	*	Constructor.
	*	@param buffer Buffer to read from.
	*	@param uiMessageSize Size of the message to read from the buffer.
	*/
	CNetBufInputStream( CNetworkBuffer& buffer, const size_t uiMessageSize )
		: m_Buffer( buffer )
		, m_uiMessageSize( uiMessageSize )
	{
		m_Buffer.PadToByte();
	}

	bool Next( const void** data, int* size ) override final;

	void BackUp( int count ) override final;

	bool Skip( int count ) override final;

	google::protobuf::int64 ByteCount() const override final { return m_ByteCount; }

private:
	CNetworkBuffer& m_Buffer;
	const size_t m_uiMessageSize;
	google::protobuf::int64 m_ByteCount = 0;

private:
	CNetBufInputStream( const CNetBufInputStream& ) = delete;
	CNetBufInputStream& operator=( const CNetBufInputStream& ) = delete;
};

/** @} */

#endif //UTILITY_CNETBUFINPUTSTREAM_H